/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 * Contributions: Timo Bingmann <timo.bingmann@student.kit.edu>
 * Contributions: Tom Hewer <tomhewer@mac.com> for Two Ray Ground Model
 *                Pavel Boyko <boyko@iitp.ru> for matrix
 */

// This whole file is copied from ns3::TwoRayGroundPropagationLossModel
// The doCalcRxPower method is changed

#include "ns3/PA_957634_newTwoRayGroundPropModel.h"
#include "ns3/log.h"
#include "ns3/mobility-model.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/pointer.h"
#include <cmath>
#include <complex>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE("NewTwoRayGroundLossModel");

// ------------------------------------------------------------------------- //
// -- Two-Ray Ground Model ported from NS-2 -- tomhewer@mac.com -- Nov09 //

NS_OBJECT_ENSURE_REGISTERED (NewTwoRayGroundPropagationLossModel);

TypeId 
NewTwoRayGroundPropagationLossModel::GetTypeId (void)
{
	static TypeId tid = TypeId ("ns3::NewTwoRayGroundPropagationLossModel")
		.SetParent<PropagationLossModel> ()
		.SetGroupName ("Propagation")
		.AddConstructor<NewTwoRayGroundPropagationLossModel> ()
		.AddAttribute ("Wavelength",
		               "The carrier wavelenght (in m) at which propagation occurs  (default is 0.58212127766990291262 aka 5.15 Ghz).",
		               DoubleValue (0.58212127766990291262),
		               MakeDoubleAccessor (&NewTwoRayGroundPropagationLossModel::SetWavelength,
						   &NewTwoRayGroundPropagationLossModel::GetWavelength),
		               MakeDoubleChecker<double> ())
		.AddAttribute ("SystemLoss", "The system loss",
		               DoubleValue (1.0),
		               MakeDoubleAccessor (&NewTwoRayGroundPropagationLossModel::m_systemLoss),
		               MakeDoubleChecker<double> ())
		.AddAttribute ("HeightAboveZ",
		               "The height of the antenna (m) above the node's Z coordinate",
		               DoubleValue (0),
		               MakeDoubleAccessor (&NewTwoRayGroundPropagationLossModel::m_heightAboveZ),
		               MakeDoubleChecker<double> ())
		.AddAttribute ("G", "combined other parameters",
				       DoubleValue (1.0),
					   MakeDoubleAccessor (&NewTwoRayGroundPropagationLossModel::m_G),
					   MakeDoubleChecker<double> ())
		.AddAttribute ("GroundPermittivity", "The permittivity of the ground",
				       DoubleValue (15.0),
					   MakeDoubleAccessor (&NewTwoRayGroundPropagationLossModel::m_e_r),
					   MakeDoubleChecker<double> ())
		.AddAttribute ("MinDistance",
		               "The distance under which the propagation model refuses to give results (m)",
		               DoubleValue (0.5),
		               MakeDoubleAccessor (&NewTwoRayGroundPropagationLossModel::SetMinDistance,
		                                   &NewTwoRayGroundPropagationLossModel::GetMinDistance),
		               MakeDoubleChecker<double> ())
	;
	return tid;
}

NewTwoRayGroundPropagationLossModel::NewTwoRayGroundPropagationLossModel ()
{
}
void
NewTwoRayGroundPropagationLossModel::SetSystemLoss (double systemLoss)
{
	m_systemLoss = systemLoss;
}
double
NewTwoRayGroundPropagationLossModel::GetSystemLoss (void) const
{
	return m_systemLoss;
}
void
NewTwoRayGroundPropagationLossModel::SetMinDistance (double minDistance)
{
	m_minDistance = minDistance;
}
double
NewTwoRayGroundPropagationLossModel::GetMinDistance (void) const
{
	return m_minDistance;
}
void
NewTwoRayGroundPropagationLossModel::SetHeightAboveZ (double heightAboveZ)
{
	m_heightAboveZ = heightAboveZ;
}

void
NewTwoRayGroundPropagationLossModel::SetWavelength(double lambda)
{
	m_lambda = lambda;
	static const double C = 299792458.0; // speed of light in vacuum
	m_frequency = C / lambda;
}

double
NewTwoRayGroundPropagationLossModel::GetWavelength (void) const
{
	return m_lambda;
}

double 
NewTwoRayGroundPropagationLossModel::DbmToW (double dbm) const
{
	double mw = std::pow (10.0,dbm / 10.0);
	return mw / 1000.0;
}

double
NewTwoRayGroundPropagationLossModel::DbmFromW (double w) const
{
	double dbm = std::log10 (w * 1000.0) * 10.0;
	return dbm;
}

double
NewTwoRayGroundPropagationLossModel::DoCalcRxPower (double txPowerDbm,
		Ptr<MobilityModel> a,
		Ptr<MobilityModel> b) const
{
	/*
	 * Nonapproximate Two-Ray Ground equation:
	 *
	 *   Pr     lambda        |  G    R * G * e^{-i*2*pi*(l_r - l_d)/lambda} |^2
	 *   -- =  (------)^2  *  | --- + ------------------------------------- |
	 *   Pt      4 pi         | l_d               l_r                       |
	 *
	 *     sin theta - sqrt(e_r - cos^2 theta)
	 * R = -----------------------------------
	 *     sin theta + sqrt(e_r - cos^2 theta)
	 *
	 * l_r = sqrt(d^2 + (h_t + h_r)^2)
	 *
	 * sin theta = (h_t + h_r) / l_r
	 * cos theta = d / l_r
	 *
	 * l_d = sqrt(d^2 + (h_t - h_r)^2)
	 *
	 * d: ground distance between rec and trans
	 * lambda: wavelength (m)
	 * R: reflection coefficient
	 * l_r: total path length of reflected ray
	 * l_d: total path length of direct ray
	 * theta: reflection angle (between ground and ray)
	 * h_t: Tx antenna height (m)
	 * h_r: Rx antenna height (m)
	 * G: other paramters combined
	 *
	 * As with the Friis model we ignore tx and rx gain and output values
	 * are in dB or dBm
	 *
	 *                        lambda^2   |  G     R * G * e^{-i*2*pi*(l_r - l_d)/lambda} |^2
	 * rx = tx + 10 * log10 ( -------- * | --- + --------------------------------------- |   )
	 *                        16 * pi    | l_d                      l_r                  |
	 */

	// ground (2D) distance, so z not needed
	Vector2D a_ground(a->GetPosition().x, a->GetPosition().y);
	Vector2D b_ground(b->GetPosition().x, b->GetPosition().y);
	double d = CalculateDistance(a_ground, b_ground);
	// (3D) distance
	double distance = a->GetDistanceFrom (b);

	if (d <= m_minDistance) {
		return txPowerDbm;
	}

	// Set the height of the Tx and Rx antennae
	double h_t = a->GetPosition ().z + m_heightAboveZ;
	double h_r = b->GetPosition ().z + m_heightAboveZ;

	double tmp = 0;
//	// Calculate a crossover distance, under which we use Friis
//	/*
//	 *
//	 * dCross = (4 * pi * Ht * Hr) / lambda
//	 *
//	 */
//
//	double dCross = (4 * M_PI * h_t * h_r) / m_lambda;
//	if (d <= 0*dCross)
//	{
//		//std::cout << "friis\n";
//		// We use Friis
//		double numerator = m_lambda * m_lambda;
//		tmp = M_PI * distance;
//		double denominator = 16 * tmp * tmp * m_systemLoss;
//		double pr = 10 * std::log10 (numerator / denominator);
//		NS_LOG_DEBUG ("Receiver within crossover (" << dCross << "m) for Two_ray path; using Friis");
//		NS_LOG_DEBUG ("distance=" << distance << "m, attenuation coefficient=" << pr << "dB");
//		return txPowerDbm + pr;
//	}
//	else   // Use Two-Ray Pathloss
	{
		double l_r = std::sqrt(std::pow(d,2) + std::pow((h_t + h_r),2));
		double sinTheta = (h_t + h_r)/l_r;
		double cosTheta = (d/l_r);
		double l_d = std::sqrt(std::pow(d,2) + std::pow((h_t - h_r),2));
		double R = (sinTheta - std::sqrt(m_e_r - std::pow(cosTheta, 2)))
			/ (sinTheta + std::sqrt(m_e_r - std::pow(cosTheta, 2)));

		std::complex<double> comp_tmp(0,-2 * M_PI * (l_d - l_r) / m_lambda);
		comp_tmp = std::exp(comp_tmp);
		comp_tmp *= R * m_G / l_r;
		comp_tmp += m_G/l_d;
		tmp = std::abs(comp_tmp);
		tmp *= m_lambda / (4 * M_PI);
		tmp *= tmp;
		double rayPr = 10 * std::log10(tmp * m_systemLoss);
		NS_LOG_DEBUG ("distance=" << distance << "m, attenuation coefficient=" << rayPr << "dB");
	    return txPowerDbm + rayPr;
	}
}

int64_t
NewTwoRayGroundPropagationLossModel::DoAssignStreams (int64_t stream)
{
	return 0;
}
}
