package benchmarks

sealed abstract class MultirunResult(val name: String)

case class MultirunSuccess(override val name: String, times: Seq[Long])
    extends MultirunResult(name)
case class MultirunFail(override val name: String) extends MultirunResult(name)
case class MultirunDisabled(override val name: String)
    extends MultirunResult(name)

object MultirunResult {

  def apply(results: Seq[BenchmarkResult]): MultirunResult = {
    if (results.isEmpty)
      throw new Exception("Multi run needs at least one result.")

    val name = results.head.name

    if (results.exists(_.isInstanceOf[BenchmarkDisabled]))
      MultirunDisabled(results.head.name)
    else {
      if (results.exists(_.isInstanceOf[BenchmarkFailed]))
        MultirunFail(results.head.name)
      else {
        MultirunSuccess(name, results.flatMap {
          case BenchmarkCompleted(_, times) => times
        })
      }
    }

  }
}
