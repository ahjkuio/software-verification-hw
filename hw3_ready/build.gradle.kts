plugins {
    kotlin("jvm") version "1.8.0"  // Используйте вашу актуальную версию Kotlin
}

repositories {
    mavenCentral()
}

// Явно указываем JDK 17 для тестов
kotlin {
    jvmToolchain(17) // Указываем совместимую версию JVM
}

// Альтернативный вариант настройки JVM target
tasks.withType<org.jetbrains.kotlin.gradle.tasks.KotlinCompile> {
    kotlinOptions {
        jvmTarget = "17" // Или можно использовать "1.8"
    }
}

tasks.withType<Test> {
    // Явно указываем использование JUnit
    useJUnit()
    
    // Настройки для LinCheck на JDK 17 
    jvmArgs = listOf(
        "-XX:+UnlockDiagnosticVMOptions",
        "--add-opens=java.base/java.lang=ALL-UNNAMED"
    )
    
    // Обеспечиваем достаточно памяти
    maxHeapSize = "2g"
    
    // Вывод результатов тестов в консоль
    testLogging {
        events("passed", "skipped", "failed")
    }
}

dependencies {
    // Используем рекомендуемую версию
    testImplementation("org.jetbrains.kotlinx:lincheck:2.16")
    testImplementation("junit:junit:4.13.2")
} 