package benchmarks

import java.lang.System.exit

object Main {
  def main(args: Array[String]): Unit = {
    val benchmarkNames = Discover.discovered.map(_.name)
    val opts           = Opts(args, benchmarkNames)

    val benchmarks = opts
      .filter(Discover.discovered)
      .map(_.apply())
      .sortBy(_.getClass.getSimpleName)

    val results = benchmarks.map { bench =>
      val iterations = if (!opts.test) bench.iterations() else 1
      bench.loop(iterations)
      bench.loop(iterations)
    }
    val success = results.forall(_.success)

    println(opts.format.show(results))

    if (success) exit(0) else exit(1)
  }
}
