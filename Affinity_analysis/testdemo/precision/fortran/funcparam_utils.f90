module funcparam_utils
  implicit none
  contains
  subroutine func(name)
  IMPLICIT NONE
    EXTERNAL name
    print *, "function param"
  end subroutine
end module