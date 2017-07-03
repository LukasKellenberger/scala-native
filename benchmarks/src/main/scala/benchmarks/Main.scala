package benchmarks

object Main {
  def main(args: Array[String]): Unit = {
    val opts = Opts(args)

    opts.output.generate(Benchmark.readResult(opts.path), opts.outputArgs)
  }
}
