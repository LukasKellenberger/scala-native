package scala.scalanative.optimizer.analysis

import scala.scalanative.nir.Type.RefKind
import scala.scalanative.nir.{Type, Val}
import scala.scalanative.optimizer.analysis.MemoryLayout.PositionedType
import scala.scalanative.util.unsupported

final case class MemoryLayout(size: Long, tys: List[PositionedType]) {

  lazy val bitmapArray: Val.Array = {
    val ptrOffsets = tys.collect {
      case MemoryLayout.Tpe(_, offset, _: RefKind) =>
        // word index, without rtti
        offset / 8 - 1
    }
    val bytes = if (ptrOffsets.isEmpty) {
      Seq(Val.Byte(0.toByte))
    } else {
      val minBits   = ptrOffsets.last + 1
      val offsetSet = ptrOffsets.toSet
      val range     = 0 until (Math.ceil(minBits / 7.0).toInt * 7)
      val boolSeq   = range.map(i => offsetSet(i))

      def toByteArray(bools: Seq[Boolean]): Seq[Byte] = bools match {
        case Nil => Nil
        case _ =>
          val (current, next) = bools.splitAt(7)

          val isLast = next match {
            case Nil => 0
            case _   => 1
          }
          current
            .foldLeft(isLast) {
              case (b, true)  => (b << 1) | 1
              case (b, false) => b << 1

            }
            .toByte +: toByteArray(next)
      }
      toByteArray(boolSeq).map(Val.Byte)
    }
    println(s"$tys $bytes")
    Val.Array(Type.Byte, bytes)
  }

}

object MemoryLayout {

  sealed abstract class PositionedType {
    def size: Long
    def offset: Long
  }

  final case class Tpe(size: Long, offset: Long, ty: Type)
      extends PositionedType
  final case class Padding(size: Long, offset: Long) extends PositionedType

  def sizeOf(ty: Type): Long = ty match {
    case primitive: Type.Primitive => math.max(primitive.width / 8, 1)
    case Type.Array(arrTy, n)      => sizeOf(arrTy) * n
    case Type.Struct(_, tys)       => MemoryLayout(tys).size
    case Type.Nothing | Type.Ptr | _: Type.Trait | _: Type.Module |
        _: Type.Class =>
      8
    case _ => unsupported(s"sizeOf $ty")
  }

  def apply(tys: Seq[Type]): MemoryLayout = {
    val (size, potys) = impl(tys, 0)

    MemoryLayout(size, potys.reverse)
  }
  private def impl(tys: Seq[Type],
                   offset: Long): (Long, List[PositionedType]) = {
    if (tys.isEmpty) {
      return (0, List())
    }

    val sizes = tys.map(sizeOf)

    def findMax(tys: Seq[Type]): Long = tys.foldLeft(0L) {
      case (acc, Type.Struct(_, innerTy)) => math.max(acc, findMax(innerTy))
      case (acc, ty)                      => math.max(acc, sizeOf(ty))
    }

    val maxSize = findMax(tys)

    val (size, positionedTypes) =
      (tys zip sizes).foldLeft((offset, List[PositionedType]())) {
        case ((index, potys), (ty, size)) if size > 0 =>
          ty match {
            case Type.Struct(_, stys) =>
              val innerAlignment = findMax(stys)
              val pad =
                if (index                    % innerAlignment == 0) 0
                else innerAlignment - (index % innerAlignment)
              val (innerSize, innerTys) = impl(stys, index + pad)

              (index + pad + innerSize,
               innerTys ::: Padding(pad, index) :: potys)

            case _ =>
              val pad = if (index % size == 0) 0 else size - (index % size)
              (index + pad + size,
               Tpe(size, index + pad, ty) :: Padding(pad, index) :: potys)

          }
        case ((index, potys), _) => (index, potys)

      }

    val finalPad = if (size % maxSize == 0) 0 else maxSize - (size % maxSize)
    val potys =
      if (finalPad > 0) {
        Padding(finalPad, size) :: positionedTypes
      } else {
        positionedTypes
      }

    (potys.foldLeft(0L) { case (acc, poty) => acc + poty.size }, potys)
  }
}
