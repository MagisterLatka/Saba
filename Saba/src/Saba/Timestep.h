#pragma once

namespace Saba {

	class Timestep
	{
	public:
		Timestep(double time)
			: m_Seconds(time) {}

		inline operator double() { return m_Seconds; }

		double GetSeconds() const { return m_Seconds; }
		double GetMillliseconds() const { return m_Seconds * 1000.0; }
	private:
		double m_Seconds;
	};

}
