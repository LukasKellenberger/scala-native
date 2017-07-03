package benchmarks

import java.nio.file.{Path, Paths}

final class Opts(val path: Path = Paths.get("."),
                 val output: BenchmarkOutput = BenchmarkOutput.defaultOutput,
                 val outputArgs: List[String] = Nil) {
  private def copy(path: Path = this.path,
                   output: BenchmarkOutput = this.output,
                   outputArgs: List[String] = this.outputArgs) =
    new Opts(path, output, outputArgs)

  def withPath(value: Path) = copy(path = value)
  def withOutput(value: BenchmarkOutput, outputArgs: List[String]) =
    copy(output = value, outputArgs = outputArgs)
}

object Opts {
  def apply(args: Array[String]): Opts = {
    def loop(opts: Opts, args: List[String]): Opts = args match {
      case "--path" :: path :: rest =>
        loop(opts.withPath(Paths.get(path)), rest)
      case "--output" :: output :: rest =>
        opts.withOutput(BenchmarkOutput(output), rest)
      case other :: _ =>
        throw new Exception("unrecognized option: " + other)
      case Nil =>
        opts
    }
    loop(new Opts(), args.toList)
  }
}
