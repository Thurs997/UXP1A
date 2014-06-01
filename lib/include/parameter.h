/*
 * parameter.h
 *
 *  Created on: 1 cze 2014
 *      Author: Łukasz Kamiński
 */

#ifndef PARAMETER_H_
#define PARAMETER_H_

class ParameterBase
{
public:
    virtual ~ParameterBase() {}
    template<class T> const T& get() const; //to be implimented after Parameter
    template<class T, class U> void setValue(const U& rhs); //to be implimented after Parameter
};

template <typename T>
class Parameter : public ParameterBase
{
public:
    Parameter(const T& rhs) :value(rhs) {}
    const T& get() const {return value;}
    void setValue(const T& rhs) {value=rhs;}
private:
    T value;
};

//Here's the trick: dynamic_cast rather than virtual
template<class T> const T& ParameterBase::get() const
{ return dynamic_cast<const Parameter<T>&>(*this).get(); }
template<class T, class U> void ParameterBase::setValue(const U& rhs)
{ return dynamic_cast<Parameter<T>&>(*this).setValue(rhs); }




#endif /* PARAMETER_H_ */
