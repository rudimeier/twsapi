/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package TestJavaClient;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.GridBagConstraints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import com.ib.client.UnderComp;

public class UnderCompDlg extends JDialog {

	private UnderComp m_underComp;

    private JTextField 	m_txtConId = new JTextField();
    private JTextField 	m_txtDelta = new JTextField();
    private JTextField 	m_txtPrice = new JTextField();

    private JButton 	m_btnOk = new JButton( "OK");
    private JButton 	m_btnReset = new JButton( "Reset");
    private JButton 	m_btnCancel = new JButton( "Cancel");

    private boolean m_ok = false;
    private boolean m_reset = false;

    private static final int COL1_WIDTH = 30 ;
    private static final int COL2_WIDTH = 100 - COL1_WIDTH ;

    public UnderCompDlg(UnderComp underComp, JDialog owner) {
    	super( owner, true);

    	m_underComp = underComp;

        // create button panel
        JPanel buttonPanel = new JPanel();
        buttonPanel.add( m_btnOk);
        buttonPanel.add( m_btnReset);
        buttonPanel.add( m_btnCancel);

        // create action listeners
        m_btnOk.addActionListener( new ActionListener() {
            public void actionPerformed( ActionEvent e) {
                onOk();
            }
        });
        m_btnReset.addActionListener( new ActionListener() {
            public void actionPerformed( ActionEvent e) {
                onReset();
            }
        });
        m_btnCancel.addActionListener( new ActionListener() {
            public void actionPerformed( ActionEvent e) {
                onCancel();
            }
        });

        java.awt.GridBagConstraints gbc = new java.awt.GridBagConstraints() ;
        gbc.fill = GridBagConstraints.BOTH ;
        gbc.anchor = GridBagConstraints.CENTER ;
        gbc.weighty = 100 ;
        gbc.fill = GridBagConstraints.BOTH ;
        gbc.gridheight = 1 ;

        // create mid panel
        IBGridBagPanel midPanel = new IBGridBagPanel();
        midPanel.setBorder(BorderFactory.createTitledBorder( "Under Comp"));
        addGBComponent(midPanel, new JLabel( "Contract Id"), gbc, COL1_WIDTH, GridBagConstraints.RELATIVE) ;
        addGBComponent(midPanel, m_txtConId, gbc, COL2_WIDTH, GridBagConstraints.REMAINDER) ;
        addGBComponent(midPanel, new JLabel( "Delta"), gbc, COL1_WIDTH, GridBagConstraints.RELATIVE) ;
        addGBComponent(midPanel, m_txtDelta, gbc, COL2_WIDTH, GridBagConstraints.REMAINDER) ;
        addGBComponent(midPanel, new JLabel( "Price"), gbc, COL1_WIDTH, GridBagConstraints.RELATIVE) ;
        addGBComponent(midPanel, m_txtPrice, gbc, COL2_WIDTH, GridBagConstraints.REMAINDER) ;


        m_txtConId.setText(Integer.toString(m_underComp.m_conId));
        m_txtDelta.setText(Double.toString(m_underComp.m_delta));
        m_txtPrice.setText(Double.toString(m_underComp.m_price));

        // create dlg box
        getContentPane().add( midPanel, BorderLayout.CENTER);
        getContentPane().add( buttonPanel, BorderLayout.SOUTH);
        setTitle( "Delta Neutral");
        pack();
    }


	private void onOk() {

		try {
			int conId = Integer.parseInt(m_txtConId.getText());
			double delta = Double.parseDouble(m_txtDelta.getText());
			double price = Double.parseDouble(m_txtPrice.getText());

			m_underComp.m_conId = conId;
			m_underComp.m_delta = delta;
			m_underComp.m_price = price;
			m_ok = true;
			setVisible( false);
		}
		catch ( Exception e) {
			Main.inform( this, "Error - " + e);
		}
	}

	private void onReset() {
		m_underComp.m_conId = 0;
		m_underComp.m_delta = 0;
		m_underComp.m_price = 0;
		m_reset = true;
		setVisible( false);
	}

	private void onCancel() {
		setVisible( false);
	}

	public boolean ok() {
		return m_ok;
	}

	public boolean reset() {
		return m_reset;
	}

    private static void addGBComponent(IBGridBagPanel panel, Component comp,
            GridBagConstraints gbc, int weightx, int gridwidth)
    {
    	gbc.weightx = weightx;
    	gbc.gridwidth = gridwidth;
    	panel.setConstraints(comp, gbc);
    	panel.add(comp, gbc);
    }

}
