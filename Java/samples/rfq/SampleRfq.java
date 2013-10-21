/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package samples.rfq;


import com.ib.client.ComboLeg;
import com.ib.client.Contract;
import com.ib.client.ContractDetails;
import com.ib.client.TickType;
import com.ib.client.UnderComp;
import com.ib.contracts.ComboContract;
import com.ib.contracts.FutContract;
import com.ib.contracts.OptContract;
import com.ib.contracts.StkContract;

import java.util.Vector;


public class SampleRfq extends SimpleWrapper {

   private enum Status { None, SecDef, SecDefFMF, Rfq, Ticks, Done, Error };

   private static final int MaskBidPrice = 1;
   private static final int MaskAskPrice = 2;
   private static final int MaskBidSize  = 4;
   private static final int MaskAskSize  = 8;

   private static final int MaskRecvAll = MaskBidPrice | MaskBidSize |
                                          MaskAskPrice | MaskAskSize ;

   private Object m_mutex = new Object();
   private Status m_status = Status.None;

   private int m_clientId;
   private int m_rfqId;
   private int m_mode;

   private Contract m_contract = null;

   private int m_underConId = 0;

   private boolean  m_needFrontMonthFuture = false;
   private Contract m_frontMonthFuture = null;
   private int m_frontMonthFutureExpiry = 0;
   private int m_frontMonthFutureMult = 0;

   private double m_bidPrice = 0;
   private double m_askPrice = 0;

   private int m_bidSize = 0;
   private int m_askSize = 0;

   private int m_receivedTicks = 0;

   public SampleRfq(int clientId, int rfqId, int mode) {
      m_clientId = clientId;
      m_rfqId = rfqId;
      m_mode = mode;
   }

   public void testOrder() throws Exception {

      int clientId = 2;		
      connect(clientId);

      if (client() != null && client().isConnected()) {

         try {

            synchronized (m_mutex) {

               if (client().serverVersion() < 42) {
                  error ("Sample will not work with TWS older that 877");
               }

               while (m_status != Status.Done &&
                     m_status != Status.Error) {

                  if (m_status == Status.None) {
                     obtainContract();
                     if (m_status != Status.Error &&
                         m_status != Status.SecDef) {
                        submitRfq();
                     }
                  }
                  m_mutex.wait();
               }
            }
         }

         finally {
            disconnect();
         } 

         if (m_status == Status.Done) {

            String msg = "Done, bid=" + m_bidSize + "@" + m_bidPrice +
                              " ask=" + m_askSize + "@" + m_askPrice;

            UnderComp underComp = m_contract.m_underComp;
            if (underComp != null) {
               msg += " DN: conId=" + underComp.m_conId
                        + " price=" + underComp.m_price
                        + " delta=" + underComp.m_delta; 
            } 

            consoleMsg(msg);
         }
      }
   }

   private void obtainContract() {

      switch (m_mode) {
      case 0:
         {
            m_contract = new StkContract("IBM");
            m_contract.m_currency = "EUR";
            break;
         }
      case 1:
         {
            m_contract = new FutContract("IBM", "200809");
            break;
         }		
      case 2:
         {
            m_contract = new OptContract("IBM", "200809", 120, "CALL");
            break;
         }		
      case 3:
         {
            m_contract = new OptContract("Z", "LIFFE", "200809", 54.75, "CALL");
            m_contract.m_currency = "GBP";
            break;
         }		
      case 4:
         {
            m_contract = new ComboContract("Z", "GBP", "LIFFE");
            m_contract.m_comboLegs = new Vector<ComboLeg>(2);
            m_contract.m_comboLegs.setSize(2);

            {
               Contract l1 = new OptContract("Z", "LIFFE", "200809", 54.75, "CALL");
               l1.m_currency = "GBP";
               submitSecDef(1, l1);
            }

            {
               Contract l2 = new OptContract("Z", "LIFFE", "200810", 55.00, "CALL");
               l2.m_currency = "GBP";
               submitSecDef(2, l2);
            }

            m_status = Status.SecDef;
            break;
         }
      case 5:
         {
            m_contract = new ComboContract("IBM");
            m_contract.m_comboLegs = new Vector<ComboLeg>(1);
            m_contract.m_comboLegs.setSize(1);

            m_contract.m_underComp = new UnderComp();
            //m_contract.m_underComp.m_delta = 0.8;
            //m_contract.m_underComp.m_price = 120;

            {
               Contract l1 = new OptContract("IBM", "200809", 120, "CALL");
               submitSecDef(1, l1);
            }

            m_status = Status.SecDef;
            break;
         }
      case 6:
         {
            m_contract = new ComboContract("RUT");
            m_contract.m_comboLegs = new Vector<ComboLeg>(1);
            m_contract.m_comboLegs.setSize(1);

            m_contract.m_underComp = new UnderComp();
            m_needFrontMonthFuture = true;

            {
               Contract l1 = new OptContract("RUT", "200809", 740, "CALL");
               submitSecDef(1, l1);
            }

            m_status = Status.SecDef;
            break;
         }
      case 7:
         {
            m_contract = new ComboContract("Z", "GBP", "LIFFE");
            m_contract.m_comboLegs = new Vector<ComboLeg>(1);
            m_contract.m_comboLegs.setSize(1);

            m_contract.m_underComp = new UnderComp();

            m_needFrontMonthFuture = true;

            {
               Contract l1 = new OptContract(
                     "Z", "LIFFE", "200808", 55.00, "CALL");
               l1.m_currency = "GBP";
               submitSecDef(1, l1);
            }

            m_status = Status.SecDef;
            break;
         }
      }
   }

   private void submitSecDef(int reqId, Contract contract) {
      consoleMsg("REQ: secDef " + reqId);
      client().reqContractDetails(reqId, contract);
   }

   private void submitRfq() {
      consoleMsg("REQ: rfq " + m_rfqId);

      m_status = m_contract.m_underComp != null ? Status.Rfq : Status.Ticks;

      client().placeOrder(m_rfqId, m_contract, new RfqOrder(m_clientId, m_rfqId, 1));
   }

   private void checkReceivedAllTicks() {

      if ((m_receivedTicks & MaskRecvAll) == MaskRecvAll) {
         m_status = Status.Done;
         m_mutex.notify();
      }
   }

   public void contractDetails(int reqId, ContractDetails contractDetails) {

      consoleMsg("contractDetails: " + reqId);

      try {

         synchronized (m_mutex) {

            if (m_status == Status.SecDef) {
               /*
                * Note: we are requesting SecDefs only if we need Combo's
                */

               int legId = reqId - 1;

               ComboLeg comboLeg = new ComboLeg(
                  contractDetails.m_summary.m_conId, /* ratio */ 1,
                  (reqId == 1 ? "BUY" : "SELL"), m_contract.m_exchange, 0);

               m_contract.m_comboLegs.set(legId, comboLeg);

               /*
                * Do we have all legs?
                */
               for (int i = 0; i < m_contract.m_comboLegs.size(); ++i) {
                  if (i == legId)
                     continue;
                  if (m_contract.m_comboLegs.get(i) == null)
                     return;
               }

               if (m_contract.m_underComp != null) {
                  /*
                   * Store underConId if needed
                   */
                  if (m_underConId == 0) {
                     m_underConId  = contractDetails.m_underConId; 
                  }

                  /*
                   * Do we need to request front month future for hedging?
                   */

                  if (m_needFrontMonthFuture) {
                     m_status = Status.SecDefFMF;

                     Contract futContract = new FutContract(
                           contractDetails.m_summary.m_symbol,
                           /* all expirations */ "",
                           contractDetails.m_summary.m_currency);

                     submitSecDef(0, futContract);
                     return;
                  }

                  consoleMsg("using " + m_underConId + " for hedging");
                  m_contract.m_underComp.m_conId = m_underConId;
               }

               /*
                * And finally submit RFQ
                */
               submitRfq();
            }
            else if (m_status == Status.SecDefFMF) {

               /*
                * Ignore unknown reqId's
                */

               if (reqId != 0) {
                  return;
               }

               /*
                * Ignore secDefs with different underConId
                */
               if (contractDetails.m_underConId != m_underConId) {
                  return;
               }

               Contract contract = contractDetails.m_summary;

               /*
                * Check if we have a better match
                */

               int contractExpiry = Integer.parseInt(contract.m_expiry);
               int contractMult = Integer.parseInt(contract.m_multiplier);

               if (m_frontMonthFuture != null) {
                  if (m_frontMonthFutureExpiry <= contractExpiry) {
                     return;
                  }
                  if (m_frontMonthFutureExpiry == contractExpiry &&
                      m_frontMonthFutureMult <= contractMult) {
                     return;
                  }
               }

               m_frontMonthFuture = contract;
               m_frontMonthFutureExpiry = contractExpiry;
               m_frontMonthFutureMult = contractMult;
            }
         }
      }
      catch (Exception e) {
         // will update status and notify main thread
         error (e.toString());
      }
   }

   public void contractDetailsEnd(int reqId) {

      consoleMsg("contractDetailsEnd: " + reqId);

      try {

         synchronized (m_mutex) {

            if (m_status == Status.SecDefFMF) {

               if (reqId != 0) {
                  // ignore details end for leg requests
                  return;
               }

               if (m_frontMonthFuture == null) {
                  error ("Could not find front month future for hedging");
                  return;
               }

               consoleMsg("using " + m_frontMonthFuture.m_conId +
                  " for hedging");

               m_contract.m_underComp.m_conId =
                  m_frontMonthFuture.m_conId;

               /*
                * And finally submit RFQ
                */
               submitRfq();
            }
         }
      }
      catch (Exception e) {
         // will update status and notify main thread
         error (e.toString());
      }
   }

   public void deltaNeutralValidation(int reqId, UnderComp underComp) {

      consoleMsg("deltaNeutralValidation:" + reqId);    	

      synchronized (m_mutex) {

         if (m_status == Status.Rfq) {

            if (reqId != m_rfqId) {
               // unexpected dn validation
               return;
            }

            // update underComp
            m_contract.m_underComp = underComp;
            m_status = Status.Ticks;
         }
      }
   }
   public void orderStatus(int orderId, String status, int filled,
         int remaining, double avgFillPrice, int permId, int parentId,
         double lastFillPrice, int clientId, String whyHeld) {

      consoleMsg("orderStatus:" + orderId + " status=" + status);    	

      synchronized (m_mutex) {
         if (status.equals("Cancelled")) {
            m_status = Status.Error;
            m_mutex.notify();
         }
      }
   }

   public void tickPrice(int tickerId, int field, double price,
         int canAutoExecute) {

      consoleMsg("tickPrice:" + tickerId + " field:" + field +
            " (" + TickType.getField(field) + ") value:" + price);

      synchronized (m_mutex) {

         if (m_status == Status.Ticks) {

            switch (field) {
            case TickType.BID:
               {
                  m_bidPrice = price;
                  m_receivedTicks |= MaskBidPrice;
                  break;
               }
            case TickType.ASK:
               {
                  m_askPrice = price;
                  m_receivedTicks |= MaskAskPrice;
                  break;
               }
            }
            checkReceivedAllTicks();
         }
      }
   }    

   public void tickSize(int tickerId, int field, int size) {

      consoleMsg("tickSize:" + tickerId + " field:" + field +
            " (" + TickType.getField(field) + ") value:" + size);

      synchronized (m_mutex) {

         if (m_status == Status.Ticks) {

            switch (field) {
            case TickType.BID_SIZE:
               {
                  m_bidSize = size;
                  if (!(m_bidSize == 0 && m_bidPrice == -1)) {
                     m_receivedTicks |= MaskBidSize;
                  }
                  break;
               }
            case TickType.ASK_SIZE:
               {
                  m_askSize = size;
                  if (!(m_askSize == 0 && m_askPrice == -1)) {
                     m_receivedTicks |= MaskAskSize;
                  }
                  break;
               }
            }
            checkReceivedAllTicks();
         }
      }
   }

   public void error(String str) {
      consoleMsg("Error=" + str);
      synchronized (m_mutex) {
         m_status = Status.Error;
         m_mutex.notify();
      }
   }

   public void error(int id, int errorCode, String errorMsg) {
      consoleMsg("Error id=" + id + " code=" + errorCode + " msg=" + errorMsg);
      if (errorCode >= 2100 && errorCode < 2200) {
         return;
      }
      synchronized (m_mutex) {
         m_status = Status.Error;
         m_mutex.notify();
      }
   }

   /* ***************************************************************
    * Main Method
    *****************************************************************/

   public static void main(String[] args) {
      try {
         int rfqId = (int) (System.currentTimeMillis() / 1000);
         int mode = (args.length > 0) ? Integer.parseInt(args[0]) : 0;
         SampleRfq ut = new SampleRfq(/* clientId */ 2, rfqId, mode);
         ut.testOrder();
      } catch (Exception e) {
         e.printStackTrace();
      }
   }
}

