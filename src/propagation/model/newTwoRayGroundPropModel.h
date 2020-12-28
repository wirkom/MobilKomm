/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Telum (www.telum.ru)
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
 * Author: Mirko H. Wagner <mirwagner@uos.de>
 */

// This whole file is copied from ns3::TwoRayGroundPropagationLossModel
// The changed parts are marked

#ifndef NEW_TWO_RAY_GROUND_LOSS_MODEL_H
#define NEW_TWO_RAY_GROUND_LOSS_MODEL_H

#include "ns3/propagation-loss-model.h"

namespace ns3 {

/**
 * \ingroup propagation
 *
 * \brief a Two-Ray Ground propagation loss model ported from NS2
 *
 * Two-ray ground reflection model.
 *
 * CHANGED:
 *
 * \f$ Pr = \frac{P_t \lambda}{4 \pi} | \frac{G}{l_d} + \frac{R \cdot G \cdot e^{-i 2 \pi (l_r-l_d)/\lambda}}{l_r} |^2 \f$
 *
 * The original equation in Rappaport's book assumes L = 1.
 * To be consistent with the free space equation, L is added here.
 *
 * Ht and Hr are set at the respective nodes z coordinate plus a model parameter
 * set via SetHeightAboveZ.
 *
 * In the implementation,  \f$ \lambda \f$ is calculated as 
 * \f$ \frac{C}{f} \f$, where  \f$ C = 299792458\f$ m/s is the speed of light in
 * vacuum, and \f$ f \f$ is the frequency in Hz which can be configured by
 * the user via the Frequency attribute.
 */
class NewTwoRayGroundPropagationLossModel : public PropagationLossModel
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  NewTwoRayGroundPropagationLossModel ();

  /**
   * \param wavelength (m)
   *
   * Set the carrier wavelength used in the TwoRayGround model 
   * calculation.
   */
  void SetWavelength (double wavelength);

  /**
   * \param systemLoss (dimension-less)
   *
   * Set the system loss used by the TwoRayGround propagation model.
   */
  void SetSystemLoss (double systemLoss);
  /**
   * \param minDistance the minimum distance
   *
   * Below this distance, the txpower is returned
   * unmodified as the rxpower.
   */
  void SetMinDistance (double minDistance);
  /**
   * \returns the minimum distance.
   */
  double GetMinDistance (void) const;

  /**
   * \returns the current wavelength (m)
   */
  double GetWavelength (void) const;

  /**
   * \returns the current system loss (dimension-less)
   */
  double GetSystemLoss (void) const;
  /**
   * \param heightAboveZ the model antenna height above the node's Z coordinate
   *
   * Set the model antenna height above the node's Z coordinate
   */
  void SetHeightAboveZ (double heightAboveZ);

private:
  /**
   * \brief Copy constructor
   *
   * Defined and unimplemented to avoid misuse
   */
  NewTwoRayGroundPropagationLossModel (const NewTwoRayGroundPropagationLossModel &);
  /**
   * \brief Copy constructor
   *
   * Defined and unimplemented to avoid misuse
   * \returns
   */
  NewTwoRayGroundPropagationLossModel & operator = (const NewTwoRayGroundPropagationLossModel &);

  virtual double DoCalcRxPower (double txPowerDbm,
                                Ptr<MobilityModel> a,
                                Ptr<MobilityModel> b) const;
  virtual int64_t DoAssignStreams (int64_t stream);

  /**
   * Transforms a Dbm value to Watt
   * \param dbm the Dbm value
   * \return the Watts
   */
  double DbmToW (double dbm) const;

  /**
   * Transforms a Watt value to Dbm
   * \param w the Watt value
   * \return the Dbm
   */
  double DbmFromW (double w) const;

  double m_lambda;        //!< the carrier wavelength
  double m_frequency;     //!< the carrier frequency
  double m_systemLoss;    //!< the system loss
  double m_minDistance;   //!< minimum distance for the model
  double m_heightAboveZ;  //!< antenna height above the node's Z coordinate
  double m_e_r;           //!< ground permittivity
  double m_G;           //!< other paramters combined
};

}
#endif /* NEW_TWO_RAY_GROUND_LOSS_MODEL_H */
