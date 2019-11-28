/*
 * Measurement vector header. Collection of measurement data.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <math.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/error.h>

#include <lwiot/util/measurementvector.h>

#include <lwiot/stl/vector.h>
#include <lwiot/stl/move.h>

namespace lwiot
{
	MeasurementVector::MeasurementVector(int num) : _measurements(stl::move(num))
	{
	}

	MeasurementVector::MeasurementVector(const stl::Vector<double> &_measurements) : _measurements(_measurements)
	{
	}

	MeasurementVector::~MeasurementVector()
	{
		this->clear();
	}

	MeasurementVector &MeasurementVector::operator=(const stl::Vector<double> &rhs)
	{
		this->_measurements = rhs;
		return *this;
	}

	bool MeasurementVector::operator==(const MeasurementVector &rhs) const
	{
		if(this == &rhs)
			return true;

		if(this->_measurements.size() != rhs._measurements.size())
			return false;

		for(auto idx = 0UL; idx < this->_measurements.size(); idx++) {
			auto &m1 = this->_measurements[idx];
			auto &m2 = rhs._measurements[idx];

			if(m1 != m2)
				return false;
		}

		return true;
	}

	bool MeasurementVector::operator!=(const MeasurementVector &rhs) const
	{
		return !(rhs == *this);
	}

	void MeasurementVector::add(const double &data)
	{
		this->_measurements.add(data);
	}

	void MeasurementVector::clear()
	{
		this->_measurements.clear();
	}

	double MeasurementVector::smooth() const
	{
		double data;

		data = 0.0F;
		for(auto& _measurement : this->_measurements) {
			auto tmp = _measurement * _measurement;
			data += tmp;
		}

		data /= this->_measurements.length();
		return sqrt(data);
	}
}
