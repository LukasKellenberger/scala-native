package benchmarks.outputs

import benchmarks.{BenchmarkOutput, MultirunResult, MultirunSuccess}


object IterationStats extends BenchmarkOutput {
  override def generate(results: Seq[MultirunResult], args: List[String]): Unit = {
    val successes = results collect { case result @ MultirunSuccess(_, _, _, iters) if iters.isDefined => result }
    successes.foreach {
      success =>
        val iterations = success.iterationsPerRun.get
        val runs = success.nbRuns
        val (avg, std) = (0 until iterations).map {
          i =>
            val values = (0 until runs).map(r => success.times(i + r * iterations))
            val avg = average(values)
            (avg, standardDeviation(values, avg))
        }.unzip

        println(s"${success.name},average,${avg.mkString(",")}")
        println(s"${success.name},stddev,${std.mkString(",")}")
    }
  }

  def standardDeviation(values: Seq[Long], avg: Double): Double = {
    val count = values.length
    Math.sqrt(values.map(t => Math.pow(t - avg, 2) / count).sum)
  }

  def average(values: Seq[Long]): Double = {
    val count = values.length.toDouble
    values.foldLeft(0.0) { _ + _ / count }
  }
}
