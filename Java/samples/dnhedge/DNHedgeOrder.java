/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package samples.dnhedge;

import com.ib.client.Order;

public class DNHedgeOrder extends Order {

   public DNHedgeOrder(int clientId, int id, int size, String account,
      String settlingFirm, int underConId, String designatedLocation) {

      m_clientId = clientId;
      m_orderId = id;
      m_permId = id;

      m_account = account;
      m_clearingIntent = "AWAY";
      m_settlingFirm = settlingFirm;

      m_orderType = "VOL";
      m_action = "BUY";
      m_totalQuantity = size;

      m_volatility = 0.1;
      m_volatilityType = 1; // daily
      m_continuousUpdate = 1;
      m_deltaNeutralOrderType = "LMT";

      m_deltaNeutralConId = underConId;
      m_deltaNeutralOpenClose = "O";
      m_deltaNeutralShortSale = true;

      m_deltaNeutralDesignatedLocation = designatedLocation;
      m_deltaNeutralShortSaleSlot =
         (m_deltaNeutralDesignatedLocation.length() == 0) ? 1 : 2;
   }
}

