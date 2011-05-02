/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.3
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package gov.nasa.gsfc.ccmc;

public class vector_fieldline {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  public vector_fieldline(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  public static long getCPtr(vector_fieldline obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        CCMCJNI.delete_vector_fieldline(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public vector_fieldline() {
    this(CCMCJNI.new_vector_fieldline__SWIG_0(), true);
  }

  public vector_fieldline(long n) {
    this(CCMCJNI.new_vector_fieldline__SWIG_1(n), true);
  }

  public long size() {
    return CCMCJNI.vector_fieldline_size(swigCPtr, this);
  }

  public long capacity() {
    return CCMCJNI.vector_fieldline_capacity(swigCPtr, this);
  }

  public void reserve(long n) {
    CCMCJNI.vector_fieldline_reserve(swigCPtr, this, n);
  }

  public boolean isEmpty() {
    return CCMCJNI.vector_fieldline_isEmpty(swigCPtr, this);
  }

  public void clear() {
    CCMCJNI.vector_fieldline_clear(swigCPtr, this);
  }

  public void add(Fieldline x) {
    CCMCJNI.vector_fieldline_add(swigCPtr, this, Fieldline.getCPtr(x), x);
  }

  public Fieldline get(int i) {
    return new Fieldline(CCMCJNI.vector_fieldline_get(swigCPtr, this, i), false);
  }

  public void set(int i, Fieldline val) {
    CCMCJNI.vector_fieldline_set(swigCPtr, this, i, Fieldline.getCPtr(val), val);
  }

}
