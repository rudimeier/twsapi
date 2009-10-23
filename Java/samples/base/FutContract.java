/*
 * FutContract.java
 */
package samples.base;

import com.ib.client.Contract;

public class FutContract extends Contract {

   public FutContract(String symbol, String expiry) {
      m_symbol = symbol;
      m_secType = "FUT";
      m_exchange = "ONE";
      m_currency = "USD";
      m_expiry = expiry;
   }

   public FutContract(String symbol, String expiry, String currency) {
      m_symbol = symbol;
      m_secType = "FUT";
      m_currency = currency;
      m_expiry = expiry;
   }
}

