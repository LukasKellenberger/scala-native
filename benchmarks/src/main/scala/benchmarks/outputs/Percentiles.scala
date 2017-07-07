package benchmarks.outputs

import benchmarks.{BenchmarkOutput, MultirunResult, MultirunSuccess}

object Percentiles extends BenchmarkOutput {
  override def generate(results: Seq[MultirunResult],
                        args: List[String]): Unit = {
    val count = args.headOption.fold(10)(_.toInt)

    val percentiles = results.collect {
      case MultirunSuccess(name, times, _, _) if times.length >= count =>
        val sortedTimes = times.sorted
        val interval    = times.length.toDouble / count
        (name,
         (0 until count).map(i => sortedTimes(Math.round(i * interval).toInt)))
    }

    percentiles.foreach {
      case (name, perc) => println(s"$name,${perc.mkString(",")}")
    }
  }
}
