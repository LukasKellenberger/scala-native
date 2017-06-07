package scala.scalanative
package optimizer
package analysis

import ClassHierarchy._
import nir._

object DynamicHashMap {
  val dynMapType: Type = Type.Struct(Global.None, Seq(Type.Int, Type.Ptr, Type.Ptr, Type.Ptr))
}

class DynamicHashMap(cls: Class, dyns: Seq[String]) {
  val methods: Seq[Method] = {
    val own  = cls.methods.filter(_.attrs.isDyn)
    val sigs = own.map(m => m.name.id).toSet
    cls.parent
      .fold(Seq.empty[Method])(_.dynmap.methods)
      .filterNot(m => sigs.contains(m.name.id)) ++ own
  }
  val ty: Type = Type.Ptr
  val value: Val =
    Val.Const(DynmethodPerfectHashMap(methods, dyns))
}
