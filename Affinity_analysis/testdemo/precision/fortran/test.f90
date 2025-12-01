program main
  use shape_utils
  ! use funcparam_utils
  implicit none
  integer, dimension(3, 4, 5) :: array3D
  real, dimension(10) :: array1D
  integer, dimension(10) :: array2D
  integer, dimension(3) :: array4D
  integer :: x1
  real :: x2
  integer :: sz
  TYPE (abc) :: abctype
  TYPE (abc), dimension(2, 3) :: abcarray(2, 3)
  sz=0

  print *, storage_size(array3D) / 8
  print *, storage_size(array1D) / 8
  print *, storage_size(x1) / 8
  print *, storage_size(x2) / 8
  print *, storage_size(abctype) / 8
  print *, storage_size(abcarray) / 8

  call absize(array3D, sz)
  print *, "szie of 3D array: ", sz

  call absize(array1D, sz)
  print *, "size of 1D array: ", sz

  call absize(abcarray, sz)
  print *, "size of abctype array: ", sz

  call xysize(x1, sz)
  print *, "size of int v: ", sz

  call xysize(x2, sz)
  print *, "size of real v: ", sz

  call xysize(abctype, sz)
  print *, "size of type v: ", sz

  call submain(array1D, array2D, array3D)

contains 
  subroutine farg()
  end subroutine
end program main