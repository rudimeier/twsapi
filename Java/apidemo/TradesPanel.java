/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package apidemo;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.util.ArrayList;
import java.util.HashMap;

import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.border.TitledBorder;
import javax.swing.table.AbstractTableModel;


import apidemo.util.HtmlButton;

import com.ib.client.CommissionReport;
import com.ib.client.Execution;
import com.ib.client.ExecutionFilter;
import com.ib.controller.NewContract;
import com.ib.controller.ApiController.ITradeReportHandler;

public class TradesPanel extends JPanel implements ITradeReportHandler {
	private ArrayList<FullExec> m_trades = new ArrayList<FullExec>();
	private HashMap<String,FullExec> m_map = new HashMap<String,FullExec>();
	private Model m_model = new Model();
	
	TradesPanel() {
		JTable table = new JTable( m_model);
		JScrollPane scroll = new JScrollPane( table);
		scroll.setBorder( new TitledBorder( "Trade Log"));
		
		HtmlButton but = new HtmlButton( "Refresh") {
			@Override public void actionPerformed() {
				onRefresh();
			}
		};

		JPanel p = new JPanel( new FlowLayout( FlowLayout.RIGHT));
		p.add( but);
		
		setLayout( new BorderLayout() );
		add( scroll);
		add( p, BorderLayout.SOUTH);
	}

	public void activated() {
		onRefresh();
	}
	
	private void onRefresh() {
		ApiDemo.INSTANCE.controller().reqExecutions( new ExecutionFilter(), this);
	}

	@Override public void tradeReport(String tradeKey, NewContract contract, Execution trade) {
		FullExec full = m_map.get( tradeKey);
		
		if (full != null) {
			full.m_trade = trade;
		}
		else {
			full = new FullExec( contract, trade);
			m_trades.add( full);
			m_map.put( tradeKey, full);
		}
		
		m_model.fireTableDataChanged();
	}
	
	@Override public void tradeReportEnd() {
	}
	
	@Override public void commissionReport(String tradeKey, CommissionReport commissionReport) {
		FullExec full = m_map.get( tradeKey);
		if (full != null) {
			full.m_commissionReport = commissionReport;
		}
	}

	private class Model extends AbstractTableModel {
		@Override public int getRowCount() {
			return m_trades.size();
		}

		@Override public int getColumnCount() {
			return 7;
		}
		
		@Override public String getColumnName(int col) {
			switch( col) {
				case 0: return "Date/Time";
				case 1: return "Account";
				case 2: return "Action";
				case 3: return "Quantity";
				case 4: return "Description";
				case 5: return "Price";
				case 6: return "Commission";
				default: return null;
			}
		}

		@Override public Object getValueAt(int row, int col) {
			FullExec full = m_trades.get( row);
			
			switch( col) {
				case 0: return full.m_trade.m_time;
				case 1: return full.m_trade.m_acctNumber;
				case 2: return full.m_trade.m_side;
				case 3: return full.m_trade.m_shares;
				case 4: return full.m_contract.description();
				case 5: return full.m_trade.m_price;
				case 6: return full.m_commissionReport != null ? full.m_commissionReport.m_commission : null;
				default: return null;
			}
		}
	}

	static class FullExec {
		NewContract m_contract;
		Execution m_trade;
		CommissionReport m_commissionReport;
		
		FullExec( NewContract contract, Execution trade) {
			m_contract = contract;
			m_trade = trade;
		}
	}
}
