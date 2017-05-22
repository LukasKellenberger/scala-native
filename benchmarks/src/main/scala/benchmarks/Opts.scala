package benchmarks

final class Opts(val format: Format = TextFormat,
                 val test: Boolean = false,
                 val names: List[String] = List()) {
  private def copy(format: Format = this.format,
                   test: Boolean = this.test,
                   names: List[String] = this.names) =
    new Opts(format, test, names)
  def withFormat(value: Format) = copy(format = value)
  def withTest(value: Boolean)  = copy(test = value)
  def withName(value: String)   = copy(names = this.names :+ value)

  def filter(
      benchmarks: Seq[BenchmarkCompanion[_]]): Seq[BenchmarkCompanion[_]] =
    names match {
      case Nil => benchmarks
      case _ =>
        val nameSet = names.toSet
        benchmarks.filter(comp => nameSet(comp.name))
    }
}

object Opts {
  def apply(args: Array[String], benchNames: Seq[String]): Opts = {
    def loop(opts: Opts, args: List[String], names: Set[String]): Opts =
      args match {
        case "--format" :: format :: rest =>
          loop(opts.withFormat(Format(format)), rest, names)
        case "--test" :: rest =>
          loop(opts.withTest(true), rest, names)
        case "--name" :: name :: rest if names(name) =>
          loop(opts.withName(name), rest, names)
        case "--name" :: name :: _ =>
          throw new Exception("unrecognized benchmark: " + name)
        case other :: _ =>
          throw new Exception("unrecognized option: " + other)
        case Nil =>
          opts
      }
    loop(new Opts(), args.toList, benchNames.toSet)
  }

}
