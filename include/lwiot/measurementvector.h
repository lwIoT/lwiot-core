/*
 * Measurement vector header. Collection of measurement data.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/error.h>
#include <lwiot/stl/vector.h>

namespace lwiot
{
	class MeasurementVector {
	public:
		explicit MeasurementVector(int num = 0);
		explicit MeasurementVector(const stl::Vector<double> &_measurements);
		virtual ~MeasurementVector();

		MeasurementVector& operator =(const stl::Vector<double>& rhs);

		void add(const double& data);
		void clear();
		double smooth() const;

		bool operator==(const MeasurementVector &rhs) const;
		bool operator!=(const MeasurementVector &rhs) const;

	private:
		stl::Vector<double> _measurements;
	};
}
