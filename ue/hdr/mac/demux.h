/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2015 Software Radio Systems Limited
 *
 * \section LICENSE
 *
 * This file is part of the srsUE library.
 *
 * srsUE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsUE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#ifndef DEMUX_H
#define DEMUX_H

#include "phy/phy.h"
#include "common/mac_interface.h"
#include "common/pdu_queue.h"
#include "common/log.h"
#include "common/qbuff.h"
#include "common/timers.h"
#include "common/pdu.h"

/* Logical Channel Demultiplexing and MAC CE dissassemble */   


namespace srsue {

class demux : public srslte::pdu_queue::process_callback
{
public:
  demux();
  void init(phy_interface_mac* phy_h_, rlc_interface_mac *rlc, srslte::log* log_h_, srslte::timers* timers_db_);

  bool     process_pdus();
  uint8_t* request_buffer(uint32_t pid, uint32_t len);
  
  void     push_pdu(uint32_t pid, uint8_t *buff, uint32_t nof_bytes);
  void     push_pdu_temp_crnti(uint32_t pid, uint8_t *buff, uint32_t nof_bytes);

  void     set_uecrid_callback(bool (*callback)(void*, uint64_t), void *arg);
  bool     get_uecrid_successful();
  
  void     process_pdu(uint8_t *pdu, uint32_t nof_bytes);
  
private:
  const static int NOF_HARQ_PID    = 8; 
  const static int MAX_PDU_LEN     = 150*1024/8; // ~ 150 Mbps  
  const static int NOF_BUFFER_PDUS = 64; // Number of PDU buffers per HARQ pid
  uint8_t bcch_buffer[1024]; // BCCH PID has a dedicated buffer
  
  bool (*uecrid_callback) (void*, uint64_t);
  void *uecrid_callback_arg; 
  
  srslte::sch_pdu mac_msg;
  srslte::sch_pdu pending_mac_msg;
  
  void process_sch_pdu(srslte::sch_pdu *pdu);
  bool process_ce(srslte::sch_subh *subheader);
  
  bool       is_uecrid_successful; 
    
  phy_interface_mac *phy_h; 
  srslte::log       *log_h;
  srslte::timers    *timers_db;
  rlc_interface_mac *rlc;
  
  // Buffer of PDUs
  srslte::pdu_queue pdus; 
};

} // namespace srsue

#endif // DEMUX_H



