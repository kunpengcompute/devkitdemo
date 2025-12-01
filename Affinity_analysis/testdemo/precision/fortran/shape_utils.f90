module shape_utils
  implicit none
  TYPE :: abc
    integer :: a
    logical :: b
    real :: c
  END TYPE

contains

  SUBROUTINE xysize(v, sz)
  implicit none
  class(*) :: v
  integer :: sz

  sz = 1
  end SUBROUTINE

  SUBROUTINE absize(v, sz) 
  implicit none
  class(*) :: v(..)
  integer :: sz

  sz = size(v)
  print *, sizeof(v)
  end SUBROUTINE

  SUBROUTINE submain(array, inbuf_r, inbuf_i) 
    implicit none
    REAL array(*) ! assumed size
    integer inbuf_r(..) ! assumed rank
    integer inbuf_i(:, :, :) ! assumed shape
    integer :: sz, tt, bb
    sz=-1

    print *, "sizeof assumed size: ", sizeof(inbuf_r)
    print *, "sizeof assumed size: ", sizeof(inbuf_i)
    ! print *, "assued array size:", kind(array)
    print *, "assued array storage_size:", storage_size(array) / 8
end SUBROUTINE

end module shape_utils