package benchmarks

object BenchmarkResult {
  private val CompletedID = 3
  private val FailedID    = 1
  private val DisabledID  = 2

  def serialize(result: BenchmarkResult): String = result match {
    case BenchmarkCompleted(name, times) =>
      s"${BenchmarkResult.CompletedID},$name,${times.mkString(":")}"
    case BenchmarkDisabled(name) =>
      s"${BenchmarkResult.DisabledID},$name"
    case BenchmarkFailed(name) =>
      s"${BenchmarkResult.FailedID},$name"
  }

  def deserialize(value: String): BenchmarkResult = {
    val values = value.split(",")
    values.head.toInt match {
      case CompletedID =>
        BenchmarkCompleted(values(1), values(2).split(":").map(_.toLong))
      case FailedID =>
        BenchmarkFailed(values(1))
      case DisabledID =>
        BenchmarkDisabled(values(1))
    }
  }

}

sealed abstract class BenchmarkResult(val name: String)

case class BenchmarkCompleted(override val name: String, timesNs: Seq[Long])
    extends BenchmarkResult(name)

case class BenchmarkFailed(override val name: String)
    extends BenchmarkResult(name)

case class BenchmarkDisabled(override val name: String)
    extends BenchmarkResult(name)
