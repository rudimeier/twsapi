/*
 * RfqOrder.java
 */
package samples.rfq;

import com.ib.client.Order;

public class RfqOrder extends Order {

   public RfqOrder(int clientId, int id, int size) {

      m_clientId = clientId;
      m_orderId = id;
      m_permId = id;            
      m_totalQuantity = size;
      m_orderType = "QUOTE";

      /*
       * Note: this will be overridden by the backend
       *       because it could not keep such order
       *       (and it does not make too much sence)
       */
      m_transmit = false;
   }
}

