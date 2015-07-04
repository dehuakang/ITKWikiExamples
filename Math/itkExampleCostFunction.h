#ifndef itkExampleCostFunction_h
#define itkExampleCostFunction_h

#include <random>
#include <cmath>
#include <vector>
#include "itkMultipleValuedCostFunction.h"

namespace itk
{
class ExampleCostFunction :
public MultipleValuedCostFunction
{
public:
  /** Standard class typedefs. */
  typedef ExampleCostFunction        Self;
  typedef MultipleValuedCostFunction Superclass;
  typedef SmartPointer<Self>         Pointer;
  typedef SmartPointer<const Self>   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ExampleCostFunction, MultipleValuedCostFunction);

  // The equation we're fitting is y=C*e^(K*x)
  // The free parameters which we're trying to fit are C and K
  // Therefore, there are two parameters
  unsigned int GetNumberOfParameters() const { return 2; }
  
  // We will take a curve with concrete values for C and K,
  // which has been corrupted by Gaussian noise, and sample
  // it at 100 points on the interval [0,1].  Each of these
  // points will produce a residual with the expected value.
  // Therefore, there are 100 values (aka residuals).
  unsigned int GetNumberOfValues() const { return 100; }

  // Calculate the residual array, given a set of parameters.
  // We take parameters[0] to be C and parameters[1] to be K.
  // Therefore, this is a matter of calculating the value of y
  // at each of the sampled points, given the provided guesses
  // for C and K, and returning the difference from the data.
  MeasureType GetValue(const ParametersType &parameters) const
    {
    MeasureType residual(this->GetNumberOfValues());
    double predictedC = parameters[0];
    double predictedK = parameters[1];
    for (unsigned int i = 0; i < 100; ++i)
      {      
      double position = double(i)/100;
      double prediction = predictedC*exp(position*predictedK);
      residual[i] = prediction - y[i];
      }
    return residual;
    }

  // The "derivative" is the Jacobian, which takes the derivative
  // of each residual with respect to each parameter.  Since this
  // class does not provide a derivative, any optimizer using this
  // cost function must be told explicitly not to ask for derivative,
  // otherwise an exception will the thrown.
  void GetDerivative(const ParametersType &parameters, DerivativeType & derivative ) const
    {
    throw ExceptionObject(__FILE__,__LINE__,"No derivative available.");
    }

protected:
  ExampleCostFunction()
    {
    // Create some data
    // Take the curve y = C*e^(K*x), add noise, and sample at 100 points on [0,1]
    // C and K are our parameters
    // In the actual data, these parameters are 5.5 and 0.5
    typename std::default_random_engine generator;
    typename std::normal_distribution<double> distribution(0.0,0.5);
    double actualC = 5.5;
    double actualK = 0.5;
    for (unsigned int i = 0; i < 100; ++i)
      {
      double position = double(i)/100;
      this->x.push_back(position);
      this->y.push_back(actualC*exp(position*actualK)+distribution(generator));
      }
    };
  ~ExampleCostFunction(){};

private:
  ExampleCostFunction(const Self &); //purposely not implemented
  void operator = (const Self &); //purposely not implemented

  // The x and y positions of the data, created in the constructor
  typename std::vector<double> x;
  typename std::vector<double> y;

};

} // end namespace itk

#endif
