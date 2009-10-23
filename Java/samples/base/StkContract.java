/*
 * StkContract.java
 */
package samples.base;

import com.ib.client.Contract;

public class StkContract extends Contract {

   public StkContract(String symbol) {
      m_symbol = symbol;
      m_secType = "STK";
      m_exchange = "SMART";
      m_currency = "USD";
   }
}

