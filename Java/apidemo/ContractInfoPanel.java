/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package apidemo;

import java.awt.BorderLayout;
import java.awt.Desktop;
import java.io.File;
import java.io.FileWriter;
import java.util.ArrayList;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

import apidemo.util.HtmlButton;
import apidemo.util.NewTabbedPanel;
import apidemo.util.NewTabbedPanel.INewTab;
import apidemo.util.TCombo;
import apidemo.util.VerticalPanel;

import com.ib.controller.ApiController.IContractDetailsHandler;
import com.ib.controller.ApiController.IFundamentalsHandler;
import com.ib.controller.NewContract;
import com.ib.controller.NewContractDetails;
import com.ib.controller.Types.FundamentalType;

public class ContractInfoPanel extends JPanel {
	private final NewContract m_contract = new NewContract();
	private final NewTabbedPanel m_requestPanels = new NewTabbedPanel();
	private final NewTabbedPanel m_resultsPanels = new NewTabbedPanel();
	
	ContractInfoPanel() {
		m_requestPanels.addTab( "Contract details", new DetailsRequestPanel() );
		m_requestPanels.addTab( "Fundamentals", new FundaRequestPanel() );
		
		setLayout( new BorderLayout() );
		add( m_requestPanels, BorderLayout.NORTH);
		add( m_resultsPanels);
	}
	
	class DetailsRequestPanel extends JPanel {
		ContractPanel m_contractPanel = new ContractPanel( m_contract);
		
		DetailsRequestPanel() {
			HtmlButton but = new HtmlButton( "Query") {
				@Override protected void actionPerformed() {
					onQuery();
				}
			};

			setLayout( new BoxLayout( this, BoxLayout.X_AXIS) );
			add( m_contractPanel);
			add( Box.createHorizontalStrut(20));
			add( but);
		}
		
		protected void onQuery() {
			m_contractPanel.onOK();
			
			DetailsResultsPanel panel = new DetailsResultsPanel();
			m_resultsPanels.addTab( m_contract.symbol() + " " + "Description", panel, true, true);
			ApiDemo.INSTANCE.controller().reqContractDetails(m_contract, panel);
		}
	}

	class DetailsResultsPanel extends JPanel implements IContractDetailsHandler {
		JLabel m_label = new JLabel();
		JTextArea m_text = new JTextArea();
		
		DetailsResultsPanel() {
			JScrollPane scroll = new JScrollPane( m_text);

			setLayout( new BorderLayout() );
			add( m_label, BorderLayout.NORTH);
			add( scroll);
		}

		@Override public void contractDetails(ArrayList<NewContractDetails> list) {
			// set label
			if (list.size() == 0) {
				m_label.setText( "No matching contracts were found");
			}
			else if (list.size() > 1) {
				m_label.setText( list.size() + " contracts returned; showing first contract only");
			}
			else {
				m_label.setText( null);
			}
			
			// set text
			if (list.size() == 0) {
				m_text.setText( null);
			}
			else {
				m_text.setText( list.get( 0).toString() );
			}
		}
	}
	
	public class FundaRequestPanel extends JPanel {
		ContractPanel m_contractPanel = new ContractPanel( m_contract);
		TCombo<FundamentalType> m_type = new TCombo<FundamentalType>( FundamentalType.values() );
		
		FundaRequestPanel() {
			HtmlButton but = new HtmlButton( "Query") {
				@Override protected void actionPerformed() {
					onQuery();
				}
			};
			
			VerticalPanel rightPanel = new VerticalPanel();
			rightPanel.add( "Report type", m_type);
			
			setLayout( new BoxLayout( this, BoxLayout.X_AXIS));
			add( m_contractPanel);
			add( Box.createHorizontalStrut(20));
			add( rightPanel);
			add( Box.createHorizontalStrut(10));
			add( but);
		}
		
		protected void onQuery() {
			m_contractPanel.onOK();
			FundaResultPanel panel = new FundaResultPanel();
			FundamentalType type = m_type.getSelectedItem();
			m_resultsPanels.addTab( m_contract.symbol() + " " + type, panel, true, true);
			ApiDemo.INSTANCE.controller().reqFundamentals( m_contract, type, panel); 
		}
	}	
	
	class FundaResultPanel extends JPanel implements INewTab, IFundamentalsHandler {
		String m_data;
		JTextArea m_text = new JTextArea();

		FundaResultPanel() {
			HtmlButton b = new HtmlButton( "View in browser") {
				@Override protected void actionPerformed() {
					onView();
				}
			};

			JScrollPane scroll = new JScrollPane( m_text);
			setLayout( new BorderLayout() );
			add( scroll);
			add( b, BorderLayout.EAST);
		}

		protected void onView() {
			try {
				File file = File.createTempFile( "tws", ".xml");
				FileWriter writer = new FileWriter( file);
				writer.write( m_text.getText() );
				writer.flush();
				writer.close();
				Desktop.getDesktop().open( file);
			}
			catch( Exception e) {
				e.printStackTrace();
			}
		}

		/** Called when the tab is first visited. */
		@Override public void activated() {
			ApiDemo.INSTANCE.controller().reqFundamentals(m_contract, FundamentalType.ReportRatios, this);
		}
		
		/** Called when the tab is closed by clicking the X. */
		@Override public void closed() {
		}

		@Override public void fundamentals(String str) {
			m_data = str;
			m_text.setText( str);
		}
	}
}
