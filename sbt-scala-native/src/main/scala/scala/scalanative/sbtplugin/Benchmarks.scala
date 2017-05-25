package scala.scalanative
package sbtplugin

import java.io._

import scala.compat.Platform.EOL
import scala.io.Source

abstract class Row(val name: String)

case class SuccessfulRow(override val name: String,
                         iter: Int,
                         timesNs: Seq[Long])
    extends Row(name)
case class FailedRow(override val name: String) extends Row(name)

object Benchmarks {
  def showResults(): Unit = {
    val dir = new File("benchmarks/target/results/");
    val names =
      dir.listFiles().map(_.toPath.getFileName.toString.replace("-", "."))
    val res = names.map { name =>
      val dir = new File("benchmarks/target/results")
      dir.mkdirs()
      val file = dir.toPath.resolve(name.replace(".", "-")).toFile

      val rows = for (line <- Source.fromFile(file).getLines()) yield {
        line.split(":").toList match {
          case "0" :: iter :: timesNs :: Nil =>
            SuccessfulRow(name, iter.toInt, timesNs.split(",").map(_.toLong))
          case _ =>
            FailedRow(name)
        }
      }
      if (rows.exists(_.isInstanceOf[FailedRow])) {
        FailedBenchmark(name)
      } else {
        val (iters, times) = rows.foldLeft((0, List[Long]())) {
          case ((totalIters, totalTimes), SuccessfulRow(_, iters, times)) =>
            (totalIters + iters, times.toList ::: totalTimes)
          case (acc, _) => acc
        }

        CompletedBenchmark(name, iters, times)
      }
    }

    println(TextFormat.show(res))

  }

}

/** A verbose human-friendly format. */
object TextFormat {

  def show(rows: Seq[Formattable]): String = {

    val title = new Formattable {
      override def elements =
        Seq("Result",
            "Name",
            "iter",
            "min [ms]",
            "max [ms]",
            "avg [ms]",
            "stddev [ms]",
            "median [ms]",
            "p05 [ms]",
            "p95 [ms]",
            "avg [ms]")
    }

    Formattable.format(title +: rows)
  }

}

/** A row in the table for a benchmark that completed. */
case class CompletedBenchmark(name: String,
                              success: String,
                              iterations: Int,
                              min: String,
                              max: String,
                              avg: String,
                              stddev: String,
                              median: String,
                              p95: String,
                              p05: String,
                              avgBetweenP05AndP95: String)
    extends Formattable {
  override def elements: Seq[String] =
    Seq(success,
        name,
        iterations.toString,
        min,
        max,
        avg,
        stddev,
        median,
        p95,
        p05,
        avgBetweenP05AndP95)
}

object CompletedBenchmark {
  def apply(name: String, iter: Long, timesNs: Seq[Long]): CompletedBenchmark = {
    import StatUtils._

    val successStr = "[OK]"
    val timesMs    = timesNs map (_ / 1e6)
    val sortedMs   = timesMs.sorted
    val minMs      = timesMs.min
    val maxMs      = timesMs.max
    val avgMs      = average(timesMs)
    val medianMs   = percentile(50, sortedMs)
    val p95Ms      = percentile(95, sortedMs)
    val p05Ms      = percentile(5, sortedMs)
    val iterations = timesNs.length
    val stddevMs =
      Math.sqrt(timesMs.map(t => Math.pow(t - avgMs, 2) / iterations).sum)
    val avgBetweenP05AndP95 =
      average(timesMs filter (t => t >= p05Ms && t <= p95Ms))

    CompletedBenchmark(
      name,
      successStr,
      iterations,
      format(minMs),
      format(maxMs),
      format(avgMs),
      format(stddevMs),
      format(medianMs),
      format(p95Ms),
      format(p05Ms),
      format(avgBetweenP05AndP95)
    )
  }
}

/** A row in the table for a benchmark that is disabled. */
case class DisabledBenchmark(name: String) extends Formattable {
  override def elements: Seq[String] =
    Seq("[???]", s"$name is disabled.")
}

/** A row in the table for a benchmark that failed during execution. */
case class FailedBenchmark(name: String) extends Formattable {
  override def elements: Seq[String] =
    Seq("[FAIL]", s"$name has failed")

}

/** Trait for objects that can be formatted into a table */
trait Formattable {
  def elements: Seq[String]

  final def format(widths: Seq[Int]): String =
    elements.zip(widths).map((Formattable.pad _).tupled).mkString(" ")
}

object Formattable {

  /** Adds spaces after `content` to make it as long as `width`. */
  def pad(content: String, width: Int): String = {
    val insert = Math.max(0, width - content.length)
    content + " " * insert
  }

  /** Returns the maximum widths for each column across all `fs`. */
  def getWidths(fs: Seq[Formattable]): Seq[Int] = {
    val rowElements = fs.map(_.elements.length).max

    (0 until rowElements).map { i =>
      fs.flatMap(_.elements.lift(i).map(_.length)).max
    }
  }

  /** Formats the sequence `fs` in a table. */
  def format(fs: Seq[Formattable]): String = {
    val widths = getWidths(fs)
    fs.map(_.format(widths)).mkString(EOL)
  }
}

/** Collection of functions useful to display the benchmark results. */
private object StatUtils {

  /** Truncates `n` at `decimals` decimals. */
  def format(n: Double, decimals: Int = 3): String = {
    val s = n.toString
    s.substring(0, s.indexOf('.') + decimals + 1)
  }

  /** Computes the `n`th percentile of `sortedData`. */
  def percentile[T](n: Int, sortedData: Seq[T]): T = {
    val index = Math.ceil(n * sortedData.length / 100.0).toInt - 1
    sortedData(index)
  }

  /** Computes the average of `data`. */
  def average(data: Seq[Double]): Double = {
    val count = data.length.toDouble
    data.foldLeft(0.0) { _ + _ / count }
  }
}
