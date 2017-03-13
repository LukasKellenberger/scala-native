package scala.scalanative
package sbtplugin
package gc

import java.io.File
import GarbageCollector._

object GarbageCollector {
  val dir = "gc"
}

case class GarbageCollector(name: String, links: Seq[String] = Nil) {
  private val specificDir = s"$dir/$name"

  /**
   *
   * Used to find files that should not be compiled
   *
   * @param path the path to the file
   * @param nativelib nativelib directory
   * @return true if path is in the nativelib gc directory but not the current GC, false otherwise
   */
  def isOtherGC(path: String, nativelib: File): Boolean = {
    val nativeGCPath = nativelib.toPath.resolve(dir)
    path.contains(nativeGCPath.toString) && !path.contains(specificDir)
  }

  def filterFiles(files: Seq[File], nativelib: File): Seq[File] =
    files.filterNot(f => isOtherGC(f.getPath().toString, nativelib))

}

final object NoGC extends GarbageCollector("nogc")
final object BoehmGC extends GarbageCollector("boehm", Seq("gc"))
