#pragma once
#include <cstdint>
#include <concepts>
#include <ostream>
#include <cmath>
#include <complex>
#include <list>
#include <algorithm>
#include <iostream>

namespace qplex {

		/************************************************
			Forward declaration
		*************************************************/

		template <std::floating_point FloatType>
		class Complex;

		/************************************************
			ComplexView
			Treat a 2-element (or strided) array as a complex number.
		*************************************************/

		template <std::floating_point FloatType, uint32_t Stride = 1>
		class ComplexView {
		public:
			ComplexView(FloatType* data) : mData{data} {}
			~ComplexView() {}
				ComplexView(ComplexView&& other) = default;
				ComplexView(const ComplexView& other) = default;

			inline FloatType& Re() { return mData[0]; }
			inline FloatType& Im() { return mData[Stride]; }
			inline const FloatType& Re() const { return mData[0]; }
			inline const FloatType& Im() const { return mData[Stride]; }

			inline ComplexView& operator~() {
				Im() = -Im();
				return *this;
			}

				inline ComplexView& operator=(const ComplexView& other) {
				Re() = other.Re();
				Im() = other.Im();
				return *this;
			}

				inline ComplexView& operator=(const Complex<FloatType>& other) {
					Re() = other.Re();
					Im() = other.Im();
					return *this;
				}

			inline ComplexView& operator!=(const ComplexView& other) {
				Re() =  other.Re();
				Im() = -other.Im();
				return *this;
			}

			inline ComplexView& operator+=(const ComplexView& other) {
				Re() += other.Re();
				Im() += other.Im();
				return *this;
			}

			inline ComplexView& operator-=(const ComplexView& other) {
				Re() -= other.Re();
				Im() -= other.Im();
				return *this;
			}

			inline ComplexView& operator*=(const ComplexView& other) {
				auto re = Re();
				auto im = Im();
				auto otherRe = other.Re();
				auto otherIm = other.Im();
				Re() = re * otherRe - im * otherIm;
				Im() = re * otherIm + im * otherRe;
				return *this;
			}

			inline ComplexView& operator/=(const ComplexView& other) {
				auto re = Re();
				auto im = Im();
				auto otherRe = other.Re();
				auto otherIm = other.Im();
				auto mag = otherRe * otherRe + otherIm * otherIm;
				Re() = (re * otherRe + im * otherIm) / mag;
				Im() = (im * otherRe - re * otherIm) / mag;
				return *this;
			}

			inline ComplexView& operator+=(const FloatType& other) {
				Re() += other;
				return *this;
			}

			inline ComplexView& operator-=(const FloatType& other) {
				Re() -= other;
				return *this;
			}

			inline ComplexView& operator*=(const FloatType& other) {
				Re() *= other;
				Im() *= other;
				return *this;
			}

			inline ComplexView& operator/=(const FloatType& other) {
				Re() /= other;
				Im() /= other;
				return *this;
			}

			inline ComplexView& operator!() const {
				Im() = -Im();
				return *this;
			}

			inline FloatType Mag2() const { return Re() * Re() + Im() * Im(); }

			inline FloatType Mag() const {
				using std::sqrt;
				return sqrt(Mag2());
			}

			inline FloatType Arg() const {
				using std::atan2;
				return atan2(Im(), Re());
			}

			inline friend std::ostream& operator<<(std::ostream& os, const ComplexView& c) {
				os << c.Re() << " + " << c.Im() << "i";
				return os;
			}

			private:
			FloatType* mData;
	};

	/************************************************
		Complex
		A complex number.
	*************************************************/

	template <std::floating_point FloatType>
	class Complex {
		public:
		Complex(const FloatType& re = FloatType{}, const FloatType& im = FloatType{}) : mData{re, im} {}
		Complex(const std::list<FloatType>& data) : mData{data[0], data[1]} {}
		Complex(const std::array<FloatType, 2>& data) : mData{data[0], data[1]} {}
		Complex(const std::complex<FloatType>& other) : mData{other.real(), other.imag()} {}

		Complex(const Complex<FloatType>& data) : mData{data.Re(), data.Im()} {}
		Complex(Complex<FloatType>&& data) : mData{data.Re(), data.Im()} {  }
		~Complex() {}

		static inline Complex const GetAs(const FloatType* data) { return Complex{data[0], data[1]}; }
		static inline auto const MultiplyAs(const ComplexView<FloatType>& left, const ComplexView<FloatType>& right) { return {left[0]*right[0] - left[1]*right[1], left[0]*right[1] + left[1]*right[0]}; }

		operator std::complex<FloatType>() const { return std::complex<FloatType>{Re(), Im()}; }


		inline FloatType& Re() { return mData[0]; }
		inline FloatType& Im() { return mData[1]; }
		inline const FloatType& Re() const { return mData[0]; }
		inline const FloatType& Im() const { return mData[1]; }

		FloatType Mag2() const { return Re() * Re() + Im() * Im(); }

		FloatType Mag() const {
			using std::sqrt;
			return sqrt(Mag2());
		}

		FloatType Arg() const {
			using std::atan2;
			return atan2(Im(), Re());
		}

		inline Complex& operator=(const Complex& other) {
			Re() = other.Re();
			Im() = other.Im();
			return *this;
		}

		inline Complex& operator!=(const Complex& other) {
			Re() =  other.Re();
			Im() = -other.Im();
			return *this;
		}

		inline Complex& operator+=(const Complex& other) {
			Re() += other.Re();
			Im() += other.Im();
			return *this;
		}

		inline Complex& operator-=(const Complex& other) {
			Re() -= other.Re();
			Im() -= other.Im();
			return *this;
		}

		inline Complex& operator*=(const Complex& other) {
			auto re = Re();
			auto im = Im();
			auto otherRe = other.Re();
			auto otherIm = other.Im();
			Re() = re * otherRe - im * otherIm;
			Im() = re * otherIm + im * otherRe;
			return *this;
		}

		inline Complex& operator/=(const Complex& other) {
			auto re = Re();
			auto im = Im();
			auto otherRe = other.Re();
			auto otherIm = other.Im();
			auto mag = otherRe * otherRe + otherIm * otherIm;
			Re() = (re*otherRe + im*otherIm)/mag;
			Im() = (im*otherRe - re*otherIm)/mag;
			return *this;
		}

		// compound ops with scalar, mirroring ComplexView semantics
		inline Complex& operator+=(const FloatType& other) {
			Re() += other;
			return *this;
		}

		inline Complex& operator-=(const FloatType& other) {
			Re() -= other;
			return *this;
		}

		inline Complex& operator*=(const FloatType& other) {
			Re() *= other;
			Im() *= other;
			return *this;
		}

		inline Complex& operator/=(const FloatType& other) {
			Re() /= other;
			Im() /= other;
			return *this;
		}

		inline friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
			os << c.Re() << " + " << c.Im() << "i";
			return os;
		}

		private:
		FloatType mData[2];
	};


		namespace ImaginaryUnit {
			inline Complex<double> operator""_i(long double im)
			{
				return Complex<double>{0.0, static_cast<double>(im)};
			}
		}

		template <std::floating_point FloatType>
			inline Complex<FloatType> operator+( const Complex<FloatType>& left, const Complex<FloatType>& right)
			{
				auto result = left;
				result += right;
				return result;
			}

		template <std::floating_point FloatType>
			inline Complex<FloatType> operator+( const FloatType &left, const Complex<FloatType>& right)
			{
				auto result = right;
				result += left;
				return result;
			}

		template <std::floating_point FloatType>
			inline Complex<FloatType> operator+( const Complex<FloatType>& left, const FloatType &right)
			{
				auto result = left;
				result += right;
				return result;
			}

		template <std::floating_point FloatType>
		inline Complex<FloatType> operator-( const Complex<FloatType>& left, const Complex<FloatType>& right)
		{
			auto result = left;
			result -= right;
			return result;
		}

		template <std::floating_point FloatType>
			inline Complex<FloatType> operator-( const FloatType &left, const Complex<FloatType>& right)
			{
				// left - (a + ib) = (left - a) - i b
				return Complex<FloatType>{left - right.Re(), -right.Im()};
			}

		template <std::floating_point FloatType>
		inline Complex<FloatType> operator-( const Complex<FloatType>& left, const FloatType &right)
		{
			auto result = left;
			result -= right;
			return result;
		}

		template <std::floating_point FloatType>
		inline Complex<FloatType> operator*( const Complex<FloatType>& left, const Complex<FloatType>& right)
		{
			auto result = left;
			result *= right;
			return result;
		}

		template <std::floating_point FloatType>
			inline Complex<FloatType> operator*( const Complex<FloatType>& left, const FloatType& right)
			{
				auto result = left;
				result *= right;
				return result;
			}

		template <std::floating_point FloatType>
			inline Complex<FloatType> operator*( const FloatType& left, const Complex<FloatType>& right)
			{
				auto result = right;
				result *= left;
				return result;
			}

		template <std::floating_point FloatType>
		inline Complex<FloatType> operator/( const Complex<FloatType>& left, const Complex<FloatType>& right)
		{
			auto result = left;
			result /= right;
			return result;
		}

		template <std::floating_point FloatType>
			inline Complex<FloatType> operator/( const Complex<FloatType>& left, const FloatType& right)
			{
				auto result = left;
				result /= right;
				return result;
			}

		template <std::floating_point FloatType>
			inline Complex<FloatType> operator/( const FloatType& left, const Complex<FloatType>& right)
			{
				// (left + 0i) / (a + ib)
				Complex<FloatType> numerator{left, FloatType{}};
				return numerator / right;
			}

		/************************************************
			Math Operations
		*************************************************/
	/************************************************
		ComplexMatrix
	*************************************************/

		// Forward declarations for backend helpers used by Matrix so that
		// we can call them before their full definitions later in this file.
		namespace backend {
			namespace elementwise {
				template <std::floating_point FloatType, uint32_t BufferSize>
				inline void AssignAdd(FloatType *thisOne, const FloatType *otherOne);

				template <std::floating_point FloatType, uint32_t BufferSize>
				inline void Addition(FloatType *result, const FloatType *thisOne, const FloatType *otherOne);

				template <std::floating_point FloatType, uint32_t BufferSize>
				inline void AssignSub(FloatType *thisOne, const FloatType *otherOne);

				template <std::floating_point FloatType, uint32_t BufferSize>
				inline void Subtraction(FloatType *result, const FloatType *thisOne, const FloatType *otherOne);

				template <std::floating_point FloatType, uint32_t BufferSize>
				inline void AssignScalarMult(FloatType *thisOne, const FloatType &scalar);

				template <std::floating_point FloatType, uint32_t BufferSize>
				inline void ScalarMultiplication(FloatType *result, const FloatType *thisOne, const FloatType &scalar);

				template <std::floating_point FloatType, uint32_t BufferSize>
				inline void AssignScalarDiv(FloatType *thisOne, const FloatType &scalar);
				
				template <std::floating_point FloatType, uint32_t BufferSize>
				inline void ScalarDivision(FloatType *result, const FloatType *thisOne, const FloatType &scalar);

				template <std::floating_point FloatType, uint32_t BufferSize>
				inline void AssignConjugate(FloatType *thisOne);

			}
			namespace matrixwise {
				
				template <std::floating_point FloatType, uint32_t Dimension>
				void MultiplyAsComplexRowMajor(FloatType *result, const FloatType *left, const FloatType *right);
				
				template <std::floating_point FloatType, uint32_t Dimension>
				inline void MultiplyAsRealRowMajor(FloatType *result, const FloatType *left, const FloatType *right);
			}
		}

	template <std::floating_point FloatType, uint32_t Dimension>
		class Matrix {

			static constexpr uint32_t kBufferStride = 1;
			static constexpr uint32_t kBufferSize = 2*Dimension*Dimension;

			public:

			Matrix() {}

			Matrix(const Matrix& other) {
				std::copy(other.mData, other.mData + kBufferSize, mData);
				
			}

			Matrix(Matrix&& other) noexcept {
				std::move(other.mData, other.mData + kBufferSize, mData);
				
			}

			~Matrix() {}

			inline ComplexView<FloatType,kBufferStride> operator[](uint32_t index) {
				const uint32_t row = index / Dimension;
				const uint32_t col = index % Dimension;
				return (*this)(row, col);
			}
			inline const ComplexView<FloatType,kBufferStride> operator[](uint32_t index) const {
				const uint32_t row = index / Dimension;
				const uint32_t col = index % Dimension;
				return (*this)(row, col);
			}

			inline ComplexView<FloatType,kBufferStride> operator()(uint32_t row, uint32_t col) {
				return ComplexView<FloatType,kBufferStride>{&mData[row * 2*Dimension+2*col]};
			}

			inline const ComplexView<FloatType,kBufferStride> operator()(uint32_t row, uint32_t col) const {
				return ComplexView<FloatType,kBufferStride>{const_cast<FloatType*>(&mData[row*2*Dimension+2*col])};
			}

			constexpr auto GetN() const { return Dimension; }

			void Print() const {
				for (uint32_t row = 0; row < Dimension; ++row) {
					for (uint32_t col = 0; col < Dimension; ++col) {
						std::cout << (*this)(row, col) << " ";
					}
					std::cout << std::endl;
				}
			}

			inline Matrix& operator=(const Matrix& other) {
				std::copy(other.mData, other.mData + kBufferSize, mData);
				return *this;
			}

			inline Matrix& operator=(Matrix&& other) noexcept {
				std::move(other.mData, other.mData + kBufferSize, mData);
				return *this;
			}

			inline Matrix& operator+=(const Matrix& other) {
				backend::elementwise::AssignAdd<FloatType, kBufferSize>(mData, other.mData);
				return *this;
			}

			inline Matrix& operator-=(const Matrix& other) {
				backend::elementwise::AssignSub<FloatType, kBufferSize>(mData, other.mData);
				return *this;
			}

			inline Matrix& operator*=(const Matrix& other) {
				using namespace backend::matrixwise;
				FloatType temp[kBufferSize];
				std::copy(mData, mData + kBufferSize, temp);
				MultiplyAsComplexRowMajor<FloatType, Dimension>(mData, temp, other.mData);

				return *this;
			}

			friend Matrix operator+(const Matrix& a, const Matrix& b) {
				Matrix result;
				backend::elementwise::Addition<FloatType, kBufferSize>(result.mData, a.mData, b.mData);
				return result;
			}

			friend Matrix operator-(const Matrix& a, const Matrix& b) {
				Matrix result;
				backend::elementwise::Subtraction<FloatType, kBufferSize>(result.mData, a.mData, b.mData);
				return result;
			}

			friend Matrix operator*(const Matrix& a, const Matrix& b) {
					
				using namespace backend::matrixwise;
				Matrix result;
				MultiplyAsComplexRowMajor<FloatType, Dimension>(result.mData, a.mData, b.mData);
				return result;
			}

			inline friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
				for (uint32_t row = 0; row < Dimension; ++row) {
					for (uint32_t col = 0; col < Dimension; ++col) {
						os << m(row, col) << " ";
					}
					os << std::endl;
				}
					return os;
			}
		private:
					FloatType mData[2*Dimension*Dimension];
			};


	namespace backend {

		namespace elementwise {

			template <std::floating_point FloatType, uint32_t BufferSize>
			inline void AssignAdd(FloatType *thisOne, const FloatType *otherOne)
			{
				std::transform(thisOne, thisOne+BufferSize, otherOne, thisOne, std::plus<>{});
			}

			template <std::floating_point FloatType, uint32_t BufferSize>
			inline void Addition(FloatType *result, const FloatType *thisOne, const FloatType *otherOne)
			{
				std::transform(thisOne, thisOne+BufferSize, otherOne, result, std::plus<>{});
			}

			template <std::floating_point FloatType, uint32_t BufferSize>
			inline void AssignSub(FloatType *thisOne, const FloatType *otherOne)
			{
				std::transform(thisOne, thisOne+BufferSize, otherOne, thisOne, std::minus<>{});
			}

			template <std::floating_point FloatType, uint32_t BufferSize>
			inline void Subtraction(FloatType *result, const FloatType *thisOne, const FloatType *otherOne)
			{
				std::transform(thisOne, thisOne+BufferSize, otherOne, result, std::minus<>{});
			}

			template <std::floating_point FloatType, uint32_t BufferSize>
			inline void AssignScalarMult(FloatType *thisOne, const FloatType &scalar)
			{
				std::transform(thisOne, thisOne + BufferSize, thisOne,
							   [scalar](FloatType x) { return x * scalar; });
			}

			template <std::floating_point FloatType, uint32_t BufferSize>
			inline void ScalarMultiplication(FloatType *result, const FloatType *thisOne, const FloatType &scalar)
			{
				std::transform(thisOne, thisOne + BufferSize, result,
							   [scalar](FloatType x) { return x*scalar; });
			}

			template <std::floating_point FloatType, uint32_t BufferSize>
			inline void AssignScalarDiv(FloatType *thisOne, const FloatType &scalar)
			{
				std::transform(thisOne, thisOne + BufferSize, thisOne,
							   [scalar](FloatType x) { return x/scalar; });
			}

			template <std::floating_point FloatType, uint32_t BufferSize>
			inline void ScalarDivision(FloatType *result, const FloatType *thisOne, const FloatType &scalar)
			{
				std::transform(thisOne, thisOne + BufferSize, result,
							   [scalar](FloatType x) { return x/scalar; });
			}
		
			template <std::floating_point FloatType, uint32_t N>
			inline void TransposeAsRealRowMajor(FloatType __restrict *result, const FloatType __restrict* thisOne)
			{
				for (uint32_t row=0; row<N; ++row) {
					uint32_t col=0;
					for (; col<N; col+=4) {
						result[row*N+col+0] = thisOne[(col+0)*N+row];
						result[row*N+col+1] = thisOne[(col+1)*N+row];
						result[row*N+col+2] = thisOne[(col+2)*N+row];
						result[row*N+col+3] = thisOne[(col+3)*N+row];
					}
					for (; col<N; ++col) {
						result[row*N+col] = thisOne[col*N+row];
					}
				}
			}

			template <std::floating_point FloatType, uint32_t N>
			inline void TransposeAsComplexRowMajor(FloatType __restrict *result, const FloatType __restrict *thisOne)
			{
				for (uint32_t row=0; row<2*N; row+=2) {
					uint32_t col=0;
					for (; col<N; col+=4) {
						result[(row+0)*N+col+0] = thisOne[(col+0)*N+row+0];
						result[(row+1)*N+col+0] = thisOne[(col+0)*N+row+1];
						result[(row+0)*N+col+1] = thisOne[(col+1)*N+row+0];
						result[(row+1)*N+col+1] = thisOne[(col+1)*N+row+1];
						result[(row+0)*N+col+2] = thisOne[(col+2)*N+row+0];
						result[(row+1)*N+col+2] = thisOne[(col+2)*N+row+1];
						result[(row+0)*N+col+3] = thisOne[(col+3)*N+row+0];
						result[(row+1)*N+col+3] = thisOne[(col+3)*N+row+1];						
					}
					for (; col<N; ++col) {
						result[(row+0)*N+col] = thisOne[col*N+row+0];
						result[(row+1)*N+col] = thisOne[col*N+row+1];
					}
				}
			}

			template <std::floating_point FloatType, uint32_t N>
			inline void ComplexConjugate(FloatType *result, const FloatType *thisOne)
			{
				for (uint32_t i=0; i<2*N*N; i+=2) {
					result[i+0] =  thisOne[i+0];
					result[i+1] = -thisOne[i+1];
				}
			}

			template <std::floating_point FloatType, uint32_t N>
			inline void TransposeAsComplexConjugateRowMajor(FloatType __restrict *result, const FloatType __restrict *thisOne)
			{
				for (uint32_t row=0; row<2*N; row+=2) {
					uint32_t col=0;
					for (; col<N; col+=4) {
						result[(row+0)*N+col+0] =  thisOne[(col+0)*N+row+0];
						result[(row+1)*N+col+0] = -thisOne[(col+0)*N+row+1];
						result[(row+0)*N+col+1] =  thisOne[(col+1)*N+row+0];
						result[(row+1)*N+col+1] = -thisOne[(col+1)*N+row+1];
						result[(row+0)*N+col+2] =  thisOne[(col+2)*N+row+0];
						result[(row+1)*N+col+2] = -thisOne[(col+2)*N+row+1];
						result[(row+0)*N+col+3] =  thisOne[(col+3)*N+row+0];
						result[(row+1)*N+col+3] = -thisOne[(col+3)*N+row+1];						
					}
					for (; col<N; ++col) {
						result[(row+0)*N+col] =  thisOne[col*N+row+0];
						result[(row+1)*N+col] = -thisOne[col*N+row+1];
					}
				}
			}
			
		}

		namespace matrixwise {

			template <std::floating_point FloatType, uint32_t N>
			inline void MultiplyAsRealRowMajor(FloatType *result, const FloatType *left, const FloatType *right)
			{
				constexpr uint32_t BufferSize = N*N;
				std::fill_n(result, BufferSize, FloatType{});

				for (uint32_t i=0; i<N; ++i) {
					FloatType* cRow = result+i*N;
					for (uint32_t k=0; k<N; ++k) {
						const FloatType a = left[i*N+k];
						const FloatType* bRow = right+k*N;

						uint32_t j = 0;
						for (; j+3<N; j+=4) {
							cRow[j+0] += a*bRow[j+0];
							cRow[j+1] += a*bRow[j+1];
							cRow[j+2] += a*bRow[j+2];
							cRow[j+3] += a*bRow[j+3];
						}

						for (; j<N; ++j) cRow[j] += a*bRow[j];
					}
				}
			}

			
			template <std::floating_point FloatType, uint32_t N>
			inline void MultiplyAsComplexRowMajor(FloatType *result, const FloatType *left, const FloatType *right)			
			{
				FloatType cRe = FloatType{};
				FloatType cIm = FloatType{};

				for (uint32_t i=0; i<N; ++i) for (uint32_t j=0; j<N; ++j) {

					cRe = FloatType{};
					cIm = FloatType{};

					const FloatType*  aPtr = left +2*i*N;
					const FloatType*  bPtr = right+2*j;

					uint32_t k=0;
					for (; k+3 < N; k+=4) {
						cRe += aPtr[0]*bPtr[0]-aPtr[1]*bPtr[1];
						cIm += aPtr[0]*bPtr[1]+aPtr[1]*bPtr[0];
						cRe += aPtr[3]*bPtr[1]-aPtr[3]*bPtr[2*N];
						cIm += aPtr[3]*bPtr[2*N]+aPtr[3]*bPtr[1];
						cRe += aPtr[4]*bPtr[4*N]-aPtr[5]*bPtr[4*N+1];
						cIm += aPtr[4]*bPtr[4*N+1]+aPtr[5]*bPtr[4*N];
						cRe += aPtr[6]*bPtr[6*N]-aPtr[7]*bPtr[6*N+1];
						cIm += aPtr[6]*bPtr[6*N+1]+aPtr[7]*bPtr[6*N];

						aPtr += 8;
						bPtr += 8*N;
					}

					for (; k < N; ++k) {
						const FloatType aRe = aPtr[0], aIm = aPtr[1];
						const FloatType bRe = bPtr[0], bIm = bPtr[1];
						cRe += aRe*bRe-aIm*bIm;
						cIm += aRe*bIm+aIm*bRe;
						aPtr += 2;
						bPtr += 2*N;
					}

					result[2*(i*N+j)]	= cRe;
					result[2*(i*N+j)+1]	= cIm;
				}
				
			}
			

			}
			
			namespace algorithms {
			
			template <std::floating_point FloatType, uint32_t N> 
			inline void Inverse(FloatType *inverted, const FloatType *input) {

				}
		}
	}
}