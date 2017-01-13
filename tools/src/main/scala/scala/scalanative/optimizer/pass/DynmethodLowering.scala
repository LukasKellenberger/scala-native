package scala.scalanative
package optimizer
package pass

import analysis.ClassHierarchy._
import nir._

/** Translates high-level structural-type method calls into
 *  low-level dispatch based on a dynmethodtable
 */
class DynmethodLowering(implicit fresh: Fresh, top: Top) extends Pass {
  import DynmethodLowering._

  override def preInst = {
    case Inst.Let(n, dyn @ Op.Dynmethod(obj, signature)) =>
      val typeptr    = Val.Local(fresh(), Type.Ptr)
      val arrPtrPtr  = Val.Local(fresh(), Type.Ptr)
      val arrPtr     = Val.Local(fresh(), Type.Ptr)
      val methptrptr = Val.Local(fresh(), Type.Ptr)

      val rtiType = Type.Struct(
        Global.None,
        Seq(Type.I32, Type.Ptr, Type.Ptr)
      )

      def throwInstrs(): Seq[Inst] = {

        val excptn = Val.Local(fresh(), Type.Class(excptnGlobal))
        val unit   = Val.Local(fresh(), Type.Unit)
        val init   = Val.Local(fresh(), Type.Ptr)

        Seq(
          Inst.Let(excptn.name, Op.Classalloc(excptnGlobal)),
          Inst.Let(
            unit.name,
            Op.Call(
              Type.Function(
                Seq(
                  Arg(Type.Class(excptnGlobal)),
                  Arg(Type.Class(Global.Top("java.lang.String")))
                ),
                Type.Unit
              ),
              Val.Global(excptnInitGlobal, Type.Ptr),
              Seq(
                excptn,
                Val.String(signature)
              )
            )
          ),
          Inst.Throw(excptn)
        )
      }

      def throwIfNull(value: Val.Local): Seq[Inst] = {
        val condNull     = Val.Local(fresh(), Type.Bool)
        val labelIsNull  = Next(fresh())
        val labelEndNull = Next(fresh())

        Seq(
          Seq(
            Inst.Let(condNull.name,
                     Op.Comp(Comp.Ieq, Type.Ptr, value, Val.Null)),
            Inst.If(condNull, labelIsNull, labelEndNull),
            Inst.Label(labelIsNull.name, Seq())
          ),
          throwInstrs(),
          Seq(
            Inst.Label(labelEndNull.name, Seq())
          )
        ).flatten
      }

      val methodIndex =
        top.dyns.zipWithIndex.find(s => s._1 == signature).get._2

      Seq(
        Seq( // Load the type information pointer
          Inst.Let(typeptr.name, Op.Load(Type.Ptr, obj)),
          Inst.Let(arrPtrPtr.name,
                   Op.Elem(rtiType, typeptr, Seq(Val.I32(0), Val.I32(2)))),
          Inst.Let(arrPtr.name, Op.Load(Type.Ptr, arrPtrPtr))
        ),
        // Test if ptr is null
        throwIfNull(arrPtr),
        Seq(
          Inst.Let(
            methptrptr.name,
            Op.Elem(Type.Struct(Global.None, top.dyns.map(_ => Type.Ptr)),
                    arrPtr,
                    Seq(Val.I32(0), Val.I32(methodIndex)))),
          Inst.Let(n, Op.Load(Type.Ptr, methptrptr))
        ),
        //Test if ptr is null
        throwIfNull(Val.Local(n, Type.Ptr))
      ).flatten

  }
}

object DynmethodLowering extends PassCompanion {
  def apply(config: tools.Config, top: Top): Pass =
    new DynmethodLowering()(top.fresh, top)

  val excptnGlobal = Global.Top("java.lang.NoSuchMethodException")
  val excptnInitGlobal =
    Global.Member(excptnGlobal, "init_class.java.lang.String")

  override def depends: Seq[Global] = Seq(excptnGlobal, excptnInitGlobal)
}
