/*
 * Measurement vector header. Collection of measurement data.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

/// @file measurementvector.h

#pragma once

# include <stdlib.h>

#include <lwiot/types.h>
#include <lwiot/error.h>

#include <lwiot/stl/vector.h>

namespace lwiot
{
	/**
	 * @brief Measurement vector.
	 * @ingroup utility
	 * @see vector
	 *
	 * Collect heterogeneous values.
	 */
	class MeasurementVector {
	public:
		/**
		 * @brief Construct a new measurement vector.
		 * @param num Number of positions to reserve.
		 */
		explicit MeasurementVector(int num = 0);
		/**
		 * @brief Copy construction.
		 * @param measurements Measurements to copy.
		 */
		explicit MeasurementVector(const stl::Vector<double> &measurements);

		/**
		 * @brief Destruct a measurement vector.
		 */
		virtual ~MeasurementVector();

		/**
		 * @brief Measurement assignment operator.
		 * @param rhs Vector to copy into \p *this.
		 * @return A reference to \p *this.
		 */
		MeasurementVector& operator =(const stl::Vector<double>& rhs);

		/**
		 * @brief Add a new measurement value to \p *this.
		 * @param data Value to add.
		 */
		void add(const double& data);

		/**
		 * @brief Clear the measurement vector.
		 */
		void clear();

		/**
		 * @brief Smooth the measurement vector values using the
		 * @return \f$ \sqrt{\frac{1}{n}\sum_{i=1}^{n}x^2} \f$
		 */
		double smooth() const;

		/**
		 * @brief Check for equality.
		 * @param rhs Object to check against.
		 * @return True or false based on success.
		 */
		bool operator==(const MeasurementVector &rhs) const;

		/**
		 * @brief Check for equality.
		 * @param rhs Object to check against.
		 * @return True or false based on success.
		 */
		bool operator!=(const MeasurementVector &rhs) const;

	private:
		stl::Vector<double> _measurements;
	};
}
