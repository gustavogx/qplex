#pragma once
#include <cstdint>
#include <concepts>
#include <ostream>
#include <cmath>
#include <complex>
#include <list>
#include <algorithm>
#include <iostream>

#ifdef QPLEX_DEBUG
	#include <iostream>
	#define CONSTRUCTED std::cout << "Constructed " << this << std::endl;
	#define DESTRUCTED std::cout << "Destructed " << this << std::endl;
	#define COPIED std::cout << "Copied " << this << std::endl;
	#define MOVED std::cout << "Moved " << this << std::endl;
#else
	#define CONSTRUCTED
	#define DESTRUCTED
	#define COPIED
	#define MOVED
#endif

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
			ComplexView(FloatType* data) : mData{data} { CONSTRUCTED }
			~ComplexView() { DESTRUCTED }
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
					Complex(const FloatType& re = FloatType{}, const FloatType& im = FloatType{}) : mData{re, im} { CONSTRUCTED }
			Complex(const std::list<FloatType>& data) : mData{data[0], data[1]} { CONSTRUCTED }
			Complex(const std::array<FloatType, 2>& data) : mData{data[0], data[1]} { CONSTRUCTED }
			Complex(const std::complex<FloatType>& other) : mData{other.real(), other.imag()} { CONSTRUCTED }
			
			Complex(const Complex<FloatType>& data) : mData{data.Re(), data.Im()} { CONSTRUCTED }
			Complex(Complex<FloatType>&& data) : mData{data.Re(), data.Im()} { MOVED }
			~Complex() { DESTRUCTED }

					
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
				Re() = (re * otherRe + im * otherIm) / mag;
				Im() = (im * otherRe - re * otherIm) / mag;
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
		// Low-level backend declarations used by MathHelper implementations.
		namespace backend {
			namespace elementwise {
				template <std::floating_point FloatType, uint32_t BufferSize>
				inline void AssignAdd(FloatType *thisOne, const FloatType *otherOne);
			}
		}
		namespace MathHelper {
		
			template <std::floating_point FloatType, uint32_t Dimension>
			inline void AssignAdd(FloatType *thisOne, const FloatType *otherOne) {
					std::transform(thisOne,
					               thisOne + 2 * Dimension * Dimension,
					               otherOne,
					               thisOne,
					               std::plus<>{});
			}

	
				template <std::floating_point FloatType, uint32_t Dimension>
				inline void Addition(FloatType *result, const FloatType *left, const FloatType *right) {						
					std::transform(left, left + Dimension*Dimension, right, result, std::plus<>{});
				}

				template <std::floating_point FloatType, uint32_t Dimension>
				inline void AssignSubtract(FloatType *thisOne, const FloatType *otherOne) {				
					std::transform(thisOne,
					               thisOne + 2 * Dimension * Dimension,
					               otherOne,
					               thisOne,
					               std::minus<>{});
				}

				template <std::floating_point FloatType, uint32_t Dimension>
				inline void Subtraction(FloatType *result, const FloatType *left, const FloatType *right) {
						constexpr uint32_t kSize = Dimension * Dimension;
						std::transform(left, left + kSize, right, result, std::minus<>{});
				}

				template <std::floating_point FloatType, uint32_t Dimension>
					inline void RealMultiply(FloatType *result, const FloatType *thisOne, const FloatType *otherOne) {
						// Each buffer represents a real-valued N x N matrix in row-major
						// order. Compute:
						//   result := thisOne * otherOne
						// using a cache-friendly i-k-j loop ordering.
						constexpr uint32_t N = Dimension;
						constexpr uint32_t kSize = N * N;
						
						// Zero the output once; the compiler can usually turn this into
						// an efficient memset.
						std::fill_n(result, kSize, FloatType{});
						
						// Standard triple-loop GEMM with inner-most j loop to keep
						// accesses to both B's row and C's row contiguous.
						for (uint32_t i = 0; i < N; ++i) {
							FloatType* cRow = result + i * N;
							for (uint32_t k = 0; k < N; ++k) {
								const FloatType a = thisOne[i * N + k];
								const FloatType* bRow = otherOne + k * N;
								// Unroll the inner loop a bit to improve ILP.
								uint32_t j = 0;
								for (; j + 3 < N; j += 4) {
									cRow[j + 0] += a * bRow[j + 0];
									cRow[j + 1] += a * bRow[j + 1];
									cRow[j + 2] += a * bRow[j + 2];
									cRow[j + 3] += a * bRow[j + 3];
								}
								for (; j < N; ++j) {
									cRow[j] += a * bRow[j];
								}
							}
						}
					}

				template <std::floating_point FloatType, uint32_t Dimension>
				inline void AssignMultiply2M(FloatType *thisOne, const FloatType *otherOne) {
					constexpr uint32_t N = Dimension;
					constexpr uint32_t kRowSpan = 2*N;
					constexpr uint32_t kBufferSize = 2*N*N;
					using View = ComplexView<FloatType, Dimension>;
					
					
					FloatType result[kBufferSize]{};
					
					for (uint32_t i = 0; i < N; ++i) {
						for (uint32_t j = 0; j < N; ++j) {
							FloatType accRe = FloatType{};
							FloatType accIm = FloatType{};
							
							for (uint32_t k = 0; k < N; ++k) {
								// A(i, k)
								FloatType* aBase = thisOne + i * kRowSpan + k;
								View a{aBase};
								// B(k, j)
								FloatType* bBase = const_cast<FloatType*>(otherOne) + k * kRowSpan + j;
								View b{bBase};
								
								auto aRe = a.Re();
								auto aIm = a.Im();
								auto bRe = b.Re();
								auto bIm = b.Im();
								
								// acc += a * b (complex multiply-add)
								accRe += aRe * bRe - aIm * bIm;
								accIm += aRe * bIm + aIm * bRe;
							}
							
							// Store into C(i, j) in the result buffer.
							FloatType* cBase = result + i * kRowSpan + j;
							View c{cBase};
							c.Re() = accRe;
							c.Im() = accIm;
						}
					}
					
					std::copy(result, result + kBufferSize, thisOne);
			}

				template <std::floating_point FloatType, uint32_t Dimension>
				inline void AssignMultiply3M(FloatType *thisOne, const FloatType *otherOne) {
					// Optimized complex GEMM using the 3M scheme:
					//
					//   A = Ar + i Ai,  B = Br + i Bi
					//   M1 = Ar * Br
					//   M2 = Ai * Bi
					//   M3 = (Ar + Ai) * (Br + Bi)
					//   Re(C) = M1 - M2
					//   Im(C) = M3 - M1 - M2
					//
					// All real multiplies are delegated to RealMultiply, which is already
					// cache-friendly and unrolled. We first "de-interleave" the real and
					// imaginary parts of the N x N complex matrices into separate
					// contiguous real N x N buffers, run the three real GEMMs, then
					// recombine back into the original complex layout.
					constexpr uint32_t N       = Dimension;
					constexpr uint32_t kRowSpan= 2 * N;
					constexpr uint32_t kSize   = N * N;
					// Separate real/imag views for A and B.
					FloatType Ar[kSize];
					FloatType Ai[kSize];
					FloatType Br[kSize];
					FloatType Bi[kSize];
					// Real GEMM results.
					FloatType M1[kSize];
					FloatType M2[kSize];
					FloatType M3[kSize];

					// De-interleave thisOne (A) and otherOne (B) into Ar/Ai and Br/Bi.
					for (uint32_t row = 0; row < N; ++row) {
						const FloatType* aRow = thisOne   + row * kRowSpan;
						const FloatType* bRow = otherOne  + row * kRowSpan;
						for (uint32_t col = 0; col < N; ++col) {
							const uint32_t idx = row * N + col;
							const FloatType aRe = aRow[col];
							const FloatType aIm = aRow[N + col];
							const FloatType bRe = bRow[col];
							const FloatType bIm = bRow[N + col];

							Ar[idx] = aRe;
							Ai[idx] = aIm;
							Br[idx] = bRe;
							Bi[idx] = bIm;
						}
					}

					// M1 = Ar * Br
					RealMultiply<FloatType, Dimension>(M1, Ar, Br);
					// M2 = Ai * Bi
					RealMultiply<FloatType, Dimension>(M2, Ai, Bi);

					// Reuse Ar/Br as temporary (Ar += Ai, Br += Bi) to form (Ar + Ai) and (Br + Bi).
					for (uint32_t idx = 0; idx < kSize; ++idx) {
						Ar[idx] += Ai[idx];
						Br[idx] += Bi[idx];
					}

					// M3 = (Ar + Ai) * (Br + Bi)
					RealMultiply<FloatType, Dimension>(M3, Ar, Br);

					// Recombine into the original complex layout of thisOne.
					for (uint32_t row = 0; row < N; ++row) {
						FloatType* cRow = thisOne + row * kRowSpan;
						for (uint32_t col = 0; col < N; ++col) {
							const uint32_t idx = row * N + col;
							const FloatType m1 = M1[idx];
							const FloatType m2 = M2[idx];
							const FloatType m3 = M3[idx];

							const FloatType re = m1 - m2;
							const FloatType im = m3 - (m1 + m2);

							cRow[col]     = re;       // Re(row, col)
							cRow[N + col] = im;       // Im(row, col)
						}
					}
				}

		
	}
	/************************************************
		ComplexMatrix		
	*************************************************/
			
	template <std::floating_point FloatType, uint32_t Dimension>
	class Matrix {
		
		public:

			Matrix() { CONSTRUCTED }
			
			Matrix(const Matrix& other) {
				std::copy(other.mData, other.mData + kBufferSize, mData);
				COPIED
			}
			
			Matrix(Matrix&& other) noexcept {
				std::move(other.mData, other.mData + kBufferSize, mData);
				MOVED
			}
			
			~Matrix() { DESTRUCTED }

			inline ComplexView<FloatType,Dimension> operator[](uint32_t index) {
				const uint32_t row = index / Dimension;
				const uint32_t col = index % Dimension;
				return (*this)(row, col);
			}
			inline const ComplexView<FloatType,Dimension> operator[](uint32_t index) const {
				const uint32_t row = index / Dimension;
				const uint32_t col = index % Dimension;
				return (*this)(row, col);
			}


				// 2D access: row/column -> ComplexView over the underlying buffer.
				// Layout per row r (0-based):
				//   [Re(r,0) .. Re(r,N-1), Im(r,0) .. Im(r,N-1)]
				// so Re is at offset (row * 2 * Dimension + col) and
				// Im is at offset (row * 2 * Dimension + Dimension + col).
				inline ComplexView<FloatType,Dimension> operator()(uint32_t row, uint32_t col) {
					return ComplexView<FloatType,Dimension>{&mData[row * 2 * Dimension + col]};
				}
				inline const ComplexView<FloatType,Dimension> operator()(uint32_t row, uint32_t col) const {
					return ComplexView<FloatType,Dimension>{const_cast<FloatType*>(&mData[row * 2 * Dimension + col])};
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
				MathHelper::AssignAdd<FloatType, Dimension>(mData, other.mData);
				return *this;
			}

			inline Matrix& operator-=(const Matrix& other) {
				MathHelper::AssignSubtract<FloatType, Dimension>(mData, other.mData);
				return *this;
			}

			inline Matrix& operator*=(const Matrix& other) {
				MathHelper::AssignMultiply3M<FloatType, Dimension>(mData, other.mData);
				return *this;
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
			static constexpr uint32_t kBufferStride = 1;
			static constexpr uint32_t kBufferSize = 2*Dimension*Dimension;
			FloatType mData[2*Dimension*Dimension];
	};

	template <std::floating_point FloatType, uint32_t Dimension>
	inline Matrix<FloatType, Dimension> operator+(const Matrix<FloatType, Dimension>& a, const Matrix<FloatType, Dimension>& b) {
			// Non-mutating matrix addition defined in terms of the
			// element-wise compound assignment operator+=.
			Matrix<FloatType, Dimension> result = a;
			result += b;
			return result;
	}

	template <std::floating_point FloatType, uint32_t Dimension>
	inline Matrix<FloatType, Dimension> operator-(const Matrix<FloatType, Dimension>& a, const Matrix<FloatType, Dimension>& b) {
			// Non-mutating matrix subtraction defined in terms of the
			// element-wise compound assignment operator-=.
			Matrix<FloatType, Dimension> result = a;
			result -= b;
			return result;
	}

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

		}
	}
}