/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.3
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package gov.nasa.gsfc.ccmc;

public class vector_string {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  public vector_string(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  public static long getCPtr(vector_string obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        CCMCJNI.delete_vector_string(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public vector_string() {
    this(CCMCJNI.new_vector_string__SWIG_0(), true);
  }

  public vector_string(long n) {
    this(CCMCJNI.new_vector_string__SWIG_1(n), true);
  }

  public long size() {
    return CCMCJNI.vector_string_size(swigCPtr, this);
  }

  public long capacity() {
    return CCMCJNI.vector_string_capacity(swigCPtr, this);
  }

  public void reserve(long n) {
    CCMCJNI.vector_string_reserve(swigCPtr, this, n);
  }

  public boolean isEmpty() {
    return CCMCJNI.vector_string_isEmpty(swigCPtr, this);
  }

  public void clear() {
    CCMCJNI.vector_string_clear(swigCPtr, this);
  }

  public void add(String x) {
    CCMCJNI.vector_string_add(swigCPtr, this, x);
  }

  public String get(int i) {
    return CCMCJNI.vector_string_get(swigCPtr, this, i);
  }

  public void set(int i, String val) {
    CCMCJNI.vector_string_set(swigCPtr, this, i, val);
  }

}
