/*
 * OptContract.java
 */
package samples.base;

import com.ib.client.Contract;

public class OptContract extends Contract {

   public OptContract(String symbol, String expiry, double strike,
         String right) {
      m_symbol = symbol;
      m_secType = "OPT";
      m_exchange = "SMART";
      m_currency = "USD";
      m_expiry = expiry;
      m_strike = strike;
      m_right = right;
   }

   public OptContract(String symbol, String exchange,
         String expiry, double strike, String right) {
      m_symbol = symbol;
      m_secType = "OPT";
      m_exchange = exchange;
      m_currency = "USD";
      m_expiry = expiry;
      m_strike = strike;
      m_right = right;
   }

}

