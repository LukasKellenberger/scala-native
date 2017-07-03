package benchmarks

import benchmarks.outputs.BasicStats

object BenchmarkOutput {
  val defaultOutput = BasicStats

  def apply(name: String): BenchmarkOutput = Map("basicstats" -> BasicStats).getOrElse(name, defaultOutput)
}

trait BenchmarkOutput {
  def generate(results: Seq[MultirunResult], args: List[String])
}
