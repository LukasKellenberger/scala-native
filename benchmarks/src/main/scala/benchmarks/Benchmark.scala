package benchmarks

import java.io._
import java.nio.file.Path

import scala.io.Source
import scala.scalanative.native.name
import scalanative.native._
@extern
object cycleclock {
  @name("scalanative_cycleclock")
  def cycleclock(): Long = extern
}

object Benchmark {
  private val resultDir = new File("benchmark-suite/results")

  def run[T](benchmark: Benchmark[T], args: Array[String]): Unit = {
    val outputFileName = args(0)
    val iterations     = args(1).toInt

    val result = benchmark.loop(iterations)
    writeResult(result, outputFileName)

  }

  private def writeResult(result: BenchmarkResult, output: String): Unit = {
    if (!resultDir.exists())
      resultDir.mkdirs()

    val outputFile = resultDir.toPath.resolve(output).toFile
    if (!outputFile.exists())
      outputFile.createNewFile()

    val pw = new PrintWriter(new FileOutputStream(outputFile, true))
    pw.println(BenchmarkResult.serialize(result))
    pw.close()
  }

  def readResult(input: Path): Seq[MultirunResult] = {
    val resultFile = resultDir.toPath.resolve(input).toFile
    if (!resultFile.exists())
      throw new Exception("Result file does not exit.")

    if (resultFile.isDirectory) {
      resultFile.listFiles().map(fileToResult)
    } else {
      Seq(fileToResult(resultFile))
    }

  }

  private def fileToResult(file: File): MultirunResult = {
    val lines = Source.fromFile(file).getLines()

    MultirunResult(lines.map(BenchmarkResult.deserialize).toSeq)
  }
}

abstract class Benchmark[T]() {
  def run(): T
  def check(t: T): Boolean

  private class BenchmarkDisabledException extends Exception
  final def disableBenchmark(): Nothing = throw new BenchmarkDisabledException

  final def loop(iterations: Int): BenchmarkResult =
    try {
      var success: Boolean   = true
      var i: Int             = 0
      val times: Array[Long] = new Array[Long](iterations)

      while (i < iterations) {
        val start  = cycleclock.cycleclock()
        val result = run()
        val end    = cycleclock.cycleclock()

        success = success && check(result)
        times(i) = end - start
        i = i + 1
      }
      if (success) {
        BenchmarkCompleted(this.getClass.getName, times)
      } else {
        BenchmarkFailed(this.getClass.getName)
      }
    } catch {
      case _: BenchmarkDisabledException =>
        BenchmarkDisabled(this.getClass.getName)
      case t: Throwable =>
        BenchmarkFailed(this.getClass.getName)
    }
}
