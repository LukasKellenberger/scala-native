package benchmarks.outputs

import benchmarks.{BenchmarkOutput, MultirunResult, MultirunSuccess}

object Percentiles extends BenchmarkOutput {

  val percc = Seq(0.5, 0.9, 0.99, 0.999, 0.9999, 0.99999)
  override def generate(results: Seq[MultirunResult],
                        args: List[String]): Unit = {
    val count = args.headOption.fold(10)(_.toInt)

    val percentiles = results.collect {
      case MultirunSuccess(name, times, _, _) if times.length >= count =>
        val sortedTimes = times.sorted
        val interval    = times.length.toDouble / count
        (name, percc.map(p => sortedTimes(Math.round(p * times.length).toInt)))
    }

    percentiles.foreach {
      case (name, perc) => println(s"$name,${perc.mkString(",")}")
    }
  }
}
