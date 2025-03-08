import org.jetbrains.kotlinx.lincheck.annotations.Operation
import org.jetbrains.kotlinx.lincheck.strategy.managed.modelchecking.ModelCheckingOptions
import org.jetbrains.kotlinx.lincheck.verifier.VerifierState
import org.junit.Test
import org.junit.Rule
import org.junit.rules.Timeout
import java.util.concurrent.TimeUnit

/**
 * Тест с использованием LinCheck для проверки свойства obstruction-freedom
 * - Использует ModelCheckingOptions.checkObstructionFreedom() для проверки
 */
class ObstructionFreeLincheckFullTest : VerifierState() {
    // Предотвращаем зависание теста
    @Rule
    @JvmField
    val timeout = Timeout(1, TimeUnit.MINUTES)

    private val deque = ObstructionFreeDeque<Int>()
    private var operationCounter = 0


    @Operation
    fun pushRight(value: Int): Boolean {
        println("[Thread ${Thread.currentThread().id}] Начало pushRight($value)")
        val result = deque.pushRight(value)
        println("[Thread ${Thread.currentThread().id}] Конец pushRight($value): $result")
        return result
    }

    @Operation
    fun pushLeft(value: Int): Boolean {
        println("[Thread ${Thread.currentThread().id}] Начало pushLeft($value)")
        val result = deque.pushLeft(value)
        println("[Thread ${Thread.currentThread().id}] Конец pushLeft($value): $result")
        return result
    }

    @Operation
    fun popRight(): Int? {
        println("[Thread ${Thread.currentThread().id}] Начало popRight()")
        val result = deque.popRight()
        println("[Thread ${Thread.currentThread().id}] Конец popRight(): $result")
        return result
    }

    @Operation
    fun popLeft(): Int? {
        println("[Thread ${Thread.currentThread().id}] Начало popLeft()")
        val result = deque.popLeft()
        println("[Thread ${Thread.currentThread().id}] Конец popLeft(): $result")
        return result
    }

    override fun extractState(): Any {
        return operationCounter++
    }

    @Test
    fun testObstructionFreedom() {
        println("проверка свойства obstruction-free")
        val options = ModelCheckingOptions()
            .iterations(8)
            .invocationsPerIteration(8)
            .threads(3)
            .actorsPerThread(3)
            .checkObstructionFreedom()
            .sequentialSpecification(ObstructionFreeDequeSequentialImpl::class.java)

        org.jetbrains.kotlinx.lincheck.LinChecker.check(this::class.java, options)
        
        println("Свойство obstruction-freedom подтверждено:")
        println("- Операции завершаются при отсутствии конкуренции от других потоков")
        println("- LinCheck убедился, что алгоритм соответствует свойству obstruction-freedom")
        
        println("\n Просто как работает очередь")
        val demoDeque = ObstructionFreeDeque<Int>()

        println("1. Добавление элементов справа")
        for (i in 1..5) {
            println("   pushRight($i): ${demoDeque.pushRight(i)}")
        }

        println("2. Добавление элементов слева")
        for (i in 6..10) {
            println("   pushLeft($i): ${demoDeque.pushLeft(i)}")
        }

        println("3. Извлечение элементов справа")
        for (i in 1..3) {
            println("   popRight(): ${demoDeque.popRight()}")
        }

        println("4. Извлечение элементов слева")
        for (i in 1..3) {
            println("   popLeft(): ${demoDeque.popLeft()}")
        }

        println("5. Текущий размер очереди: ${demoDeque.size()}")
    }
}

// Отдельная последовательная реализация для тестирования obstruction-freedom
class ObstructionFreeDequeSequentialImpl {
    private val items = ArrayList<Int>()

    @Operation
    fun pushRight(value: Int): Boolean {
        items.add(value)
        return true
    }

    @Operation
    fun pushLeft(value: Int): Boolean {
        items.add(0, value)
        return true
    }

    @Operation
    fun popRight(): Int? {
        if (items.isEmpty()) return null
        return items.removeAt(items.size - 1)
    }

    @Operation
    fun popLeft(): Int? {
        if (items.isEmpty()) return null
        return items.removeAt(0)
    }
}