import java.util.concurrent.atomic.AtomicReferenceArray
import java.util.concurrent.atomic.AtomicInteger

/**
 * Реализация obstruction-free двусторонней очереди
 */
class ObstructionFreeDeque<T> {
    private val MAX_SIZE = 1000
    private val items = AtomicReferenceArray<Any?>(MAX_SIZE)
    private val head = AtomicInteger(MAX_SIZE / 2)
    private val tail = AtomicInteger(MAX_SIZE / 2)

    init {
        println("Инициализация ObstructionFreeDeque")
        for (i in 0 until MAX_SIZE) {
            items.set(i, null)
        }
        println("Инициализация завершена: head=${head.get()}, tail=${tail.get()}")
    }

    fun pushRight(item: T): Boolean {
        if (item == null) return false

        var attempts = 0
        while (true) {
            if (attempts++ > 1000) {
                Thread.yield()
                if (attempts > 10000) return false
            }

            val currentTail = tail.get()
            val currentHead = head.get()

            // Проверка границ
            if (currentTail >= MAX_SIZE - 1 || currentTail - currentHead >= MAX_SIZE - 2) {
                return false
            }

            // Пытаемся добавить элемент и обновить tail атомарно
            if (items.compareAndSet(currentTail, null, item)) {
                // Обновляем tail, но не блокируемся если не удалось
                for (i in 0 until 3) {
                    if (tail.compareAndSet(currentTail, currentTail + 1)) break
                    Thread.yield()
                }

                // Принудительно обновляем tail если не удалось через CAS
                if (tail.get() == currentTail) {
                    tail.set(currentTail + 1)
                }

                return true
            } else if (items.get(currentTail) != null) {
                // Если позиция занята, пытаемся сдвинуть tail
                tail.compareAndSet(currentTail, currentTail + 1)
                Thread.yield()
            }
        }
    }

    fun pushLeft(item: T): Boolean {
        if (item == null) return false

        var attempts = 0
        while (true) {
            if (attempts++ > 1000) {
                Thread.yield()
                if (attempts > 10000) return false
            }

            val currentHead = head.get()
            val currentTail = tail.get()

            // Проверка границ
            if (currentHead <= 1 || currentTail - currentHead >= MAX_SIZE - 2) {
                return false
            }

            val newHead = currentHead - 1

            // Пытаемся добавить элемент и обновить head атомарно
            if (items.compareAndSet(newHead, null, item)) {
                // Обновляем head, но не блокируемся если не удалось
                for (i in 0 until 3) {
                    if (head.compareAndSet(currentHead, newHead)) break
                    Thread.yield()
                }

                // Принудительно обновляем head если не удалось через CAS
                if (head.get() == currentHead) {
                    head.set(newHead)
                }

                return true
            } else if (items.get(newHead) != null) {
                // Если позиция занята, выбираем другую
                for (offset in 1..5) {
                    val alternativeHead = currentHead - offset - 1
                    if (alternativeHead > 0 && items.get(alternativeHead) == null) {
                        head.compareAndSet(currentHead, alternativeHead)
                        break
                    }
                }
                Thread.yield()
            }
        }
    }

    fun popRight(): T? {
        var attempts = 0
        while (true) {
            if (attempts++ > 1000) {
                Thread.yield()
                if (attempts > 10000) return null
            }

            val currentTail = tail.get()
            val currentHead = head.get()

            // Ничего нет в очереди
            if (currentHead >= currentTail) {
                val itemsFound = scanForItems(currentHead - 40, currentTail + 40)
                if (!itemsFound) return null
                Thread.yield()
                continue
            }

            // Сначала проверяем обычный диапазон
            val searchPos = currentTail - 1
            if (searchPos >= currentHead) {
                val item = items.get(searchPos)
                if (item != null && items.compareAndSet(searchPos, item, null)) {
                    // Обновляем tail только если извлекли из самого конца
                    if (searchPos == currentTail - 1) {
                        tail.compareAndSet(currentTail, searchPos)
                    }
                    return item as T
                }
            }

            // Расширенный поиск
            for (i in 1..30) {
                val pos = currentTail - 1 - i
                if (pos < currentHead) break

                val item = items.get(pos)
                if (item != null && items.compareAndSet(pos, item, null)) {
                    return item as T
                }
            }

            // Если очередь реально пуста, вернем null
            val itemsFound = scanForItems(Math.max(0, currentHead - 20), Math.min(MAX_SIZE - 1, currentTail + 20))
            if (!itemsFound) {
                return null
            }

            Thread.yield()
        }
    }

    fun popLeft(): T? {
        var attempts = 0
        while (true) {
            if (attempts++ > 1000) {
                Thread.yield()
                if (attempts > 10000) return null
            }

            val currentHead = head.get()
            val currentTail = tail.get()

            // Ничего нет в очереди
            if (currentHead >= currentTail) {
                val itemsFound = scanForItems(currentHead - 40, currentTail + 40)
                if (!itemsFound) return null
                Thread.yield()
                continue
            }

            // Сначала проверяем обычный диапазон
            if (currentHead < currentTail) {
                val item = items.get(currentHead)
                if (item != null && items.compareAndSet(currentHead, item, null)) {
                    // Обновляем head только если извлекли из самого начала
                    head.compareAndSet(currentHead, currentHead + 1)
                    return item as T
                }
            }

            // Проверяем расширенный диапазон ниже head
            for (pos in Math.max(0, currentHead - 40) until currentHead) {
                val item = items.get(pos)
                if (item != null && items.compareAndSet(pos, item, null)) {
                    return item as T
                }
            }

            // Проверяем расширенный диапазон выше head
            for (pos in currentHead + 1 until Math.min(currentTail, currentHead + 40)) {
                val item = items.get(pos)
                if (item != null && items.compareAndSet(pos, item, null)) {
                    return item as T
                }
            }

            // Если очередь реально пуста, вернем null
            val itemsFound = scanForItems(Math.max(0, currentHead - 40), Math.min(MAX_SIZE - 1, currentTail + 40))
            if (!itemsFound) {
                return null
            }

            Thread.yield()
        }
    }

     //Сканируем диапазон индексов на наличие элементов
    private fun scanForItems(from: Int, to: Int): Boolean {
        val start = Math.max(0, from)
        val end = Math.min(MAX_SIZE - 1, to)

        for (pos in start..end) {
            if (items.get(pos) != null) {
                return true
            }
        }
        return false
    }

    fun size(): Int {
        return Math.max(0, tail.get() - head.get())
    }

    fun isEmpty(): Boolean {
        return size() == 0
    }
}
