#pragma once
#include <cstdint>
#include <concepts>
#include <ostream>

namespace qplex {

	template <std::floating_point FloatType, uint32_t Stride = 1>
	class ComplexView {
		public:
			ComplexView(FloatType* data) : mData{data} {}
			inline FloatType& Re() { return mData[0]; }
			inline FloatType& Im() { return mData[Stride]; }
			inline const FloatType& Re() const { return mData[0]; }
			inline const FloatType& Im() const { return mData[Stride]; }
			
			ComplexView(const ComplexView& other) = delete;			

			inline ComplexView& operator~() {
				Im() = -Im();
				return *this;
			}

			inline ComplexView& operator=(const ComplexView& other) {
				Re() = other.Re();
				Im() = other.Im();
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

			inline friend std::ostream& operator<<(std::ostream& os, const ComplexView& c) {
				os << c.Re() << " + " << c.Im() << "i";
				return os;
			}
		
			private:
			FloatType* mData;
	};

	
}