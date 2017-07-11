package benchmarks.outputs

import benchmarks.{BenchmarkOutput, MultirunResult, MultirunSuccess}

object IterationStats extends BenchmarkOutput {
  override def generate(results: Seq[MultirunResult],
                        args: List[String]): Unit = {
    val successes = results collect {
      case result @ MultirunSuccess(_, _, _, iters) if iters.isDefined =>
        result
    }
    val values = successes.map { success =>
      val iterations = success.iterationsPerRun.get
      val runs       = success.nbRuns
      (success.name, (0 until iterations).map { i =>
        (0 until runs).map(r => success.times(i + r * iterations))
      })
    }

    values.foreach {
      case (name, vals) =>
        val avgs = vals.map(average)
        val stds = (vals zip avgs).map(t => standardDeviation(t._1, t._2))
        val maxs = (vals zip avgs).map(t => t._1.max - t._2)
        val mins = (vals zip avgs).map(t => t._2 - t._1.min)

        println(s"$name,average,${avgs.mkString(",")}")
        println(s"$name,stddev,${stds.mkString(",")}")
        println(s"$name,max,${maxs.mkString(",")}")
        println(s"$name,min,${mins.mkString(",")}")

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
