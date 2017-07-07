package benchmarks

import benchmarks.outputs.{BasicStats, IterationStats, Percentiles}

object BenchmarkOutput {
  val defaultOutput = BasicStats

  def apply(name: String): BenchmarkOutput = {
    Map("basicstats"  -> BasicStats,
        "percentiles" -> Percentiles,
        "iterations"  -> IterationStats)
      .getOrElse(name, defaultOutput)
  }
}

trait BenchmarkOutput {
  def generate(results: Seq[MultirunResult], args: List[String])
}
