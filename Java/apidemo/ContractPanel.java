/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package apidemo;

import java.awt.BorderLayout;
import java.awt.Dimension;

import javax.swing.JPanel;

import apidemo.util.TCombo;
import apidemo.util.UpperField;
import apidemo.util.VerticalPanel;

import com.ib.controller.NewContract;
import com.ib.controller.Types.Right;
import com.ib.controller.Types.SecType;

public class ContractPanel extends JPanel {
	protected UpperField m_symbol = new UpperField();
	protected TCombo<SecType> m_secType = new TCombo<SecType>( SecType.values() );
	protected UpperField m_expiry = new UpperField();
	protected UpperField m_strike = new UpperField();
	protected TCombo<Right> m_right = new TCombo<Right>( Right.values() );
	protected UpperField m_multiplier = new UpperField();
	protected UpperField m_exchange = new UpperField();
	protected UpperField m_currency = new UpperField();
	protected UpperField m_localSymbol = new UpperField();
	protected UpperField m_tradingClass = new UpperField();

	private NewContract m_contract;

	ContractPanel(NewContract c) {
		m_contract = c;

		if (c.secType() == SecType.None) {
			m_symbol.setText( "IBM");
			m_secType.setSelectedItem( SecType.STK);
			m_exchange.setText( "SMART"); 
			m_currency.setText( "USD");
		}
		else {
			m_symbol.setText( m_contract.symbol());
			m_secType.setSelectedItem( m_contract.secType() );
			m_expiry.setText( m_contract.expiry());
			m_strike.setText( "" + m_contract.strike() );
			m_right.setSelectedItem( m_contract.right() ); 
			m_multiplier.setText( m_contract.multiplier() );
			m_exchange.setText( m_contract.exchange()); 
			m_currency.setText( m_contract.currency());
			m_localSymbol.setText( m_contract.localSymbol());
			m_tradingClass.setText( m_contract.tradingClass() );
		}
		
		VerticalPanel p = new VerticalPanel();
    	p.add( "Symbol", m_symbol);
    	p.add( "Sec type", m_secType);
    	p.add( "Expiry", m_expiry);
    	p.add( "Strike", m_strike);
    	p.add( "Put/call", m_right);
    	p.add( "Multiplier", m_multiplier);
    	p.add( "Exchange", m_exchange);
    	p.add( "Currency", m_currency);
    	p.add( "Local symbol", m_localSymbol);
    	p.add( "Trading class", m_tradingClass);
    	
    	setLayout( new BorderLayout() );
    	add( p);
	}
	
	@Override public Dimension getMaximumSize() {
		return super.getPreferredSize();
	}
	
	public void onOK() {
		if (m_contract.isCombo() ) {
			return;
		}
		
		m_contract.symbol( m_symbol.getText().toUpperCase() ); 
		m_contract.secType( m_secType.getSelectedItem() ); 
		m_contract.expiry( m_expiry.getText() ); 
		m_contract.strike( m_strike.getDouble() ); 
		m_contract.right( m_right.getSelectedItem() ); 
		m_contract.multiplier( m_multiplier.getText() ); 
		m_contract.exchange( m_exchange.getText().toUpperCase() ); 
		m_contract.currency( m_currency.getText().toUpperCase() ); 
		m_contract.localSymbol( m_localSymbol.getText().toUpperCase() );
		m_contract.tradingClass( m_tradingClass.getText().toUpperCase() );
	}
}
