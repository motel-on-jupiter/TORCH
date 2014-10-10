/**
 * Copyright (C) 2014 The Motel on Jupiter
 */
#ifndef AUXILIARY_ASSERTAUX_H_
#define AUXILIARY_ASSERTAUX_H_

extern void ASSERT(bool expr);
extern void assert_not_reached();
template<class T> extern void assert_equal(T expected, T actual) {
  ASSERT(expected == actual);
}
template<class T> extern void assert_not_equal(T expected, T actual) {
  assert_equal(!expected, actual);
}
template<class T> extern void assert_null(T *expr) {
  assert_equal<T *>(expr, nullptr);
}
template<class T> extern void assert_nonnull(T *expr) {
  assert_not_equal<T *>(expr, nullptr);
}

#endif /* AUXILIARY_ASSERTAUX_H_ */
