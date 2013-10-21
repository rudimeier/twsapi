/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package apidemo;

import static com.ib.controller.Formats.fmt;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.Box;
import javax.swing.JCheckBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.SwingUtilities;

import apidemo.util.HtmlButton;
import apidemo.util.NewTabbedPanel;
import apidemo.util.TCombo;
import apidemo.util.UpperField;
import apidemo.util.Util;
import apidemo.util.VerticalPanel;
import apidemo.util.VerticalPanel.HorzPanel;
import apidemo.util.VerticalPanel.StackPanel;

import com.ib.client.TagValue;
import com.ib.controller.ApiController.IOrderHandler;
import com.ib.controller.NewContract;
import com.ib.controller.NewOrder;
import com.ib.controller.NewOrderState;
import com.ib.controller.OrderType;
import com.ib.controller.Types.Action;
import com.ib.controller.Types.AlgoParam;
import com.ib.controller.Types.AlgoStrategy;
import com.ib.controller.Types.ComboParam;
import com.ib.controller.Types.HedgeType;
import com.ib.controller.Types.Method;
import com.ib.controller.Types.OcaType;
import com.ib.controller.Types.ReferencePriceType;
import com.ib.controller.Types.Rule80A;
import com.ib.controller.Types.TimeInForce;
import com.ib.controller.Types.TriggerMethod;
import com.ib.controller.Types.VolatilityType;

public class TicketDlg extends JDialog {
	private boolean m_editContract;
	private final NewContract m_contract;
	private final NewOrder m_order;
	private final ContractPanel m_contractPanel;
	private final OrderPanel m_orderPanel;
	private final AdvisorTicketPanel m_advisorPanel;
	private final MiscTicketPanel m_attribTicketPanel;
	private final VolatilityTicketPanel m_volPanel;
	private final ComboTicketPanel m_comboPanel;
	private final AlgoPanel m_algoPanel;
	private final ScalePanel m_scalePanel;
	
	public TicketDlg(NewContract contract, NewOrder order) {
		super( ApiDemo.INSTANCE.frame());
		
		if (contract == null) {
			contract = new NewContract();
			m_editContract = true;
		}

		if (order == null) {
			order = new NewOrder();
			order.totalQuantity( 100);
			order.lmtPrice( 1);
		}
		
		m_contract = contract;
		m_order = order;
		
		m_contractPanel = new ContractPanel( m_contract);
		m_orderPanel = new OrderPanel();
		m_advisorPanel = new AdvisorTicketPanel();
		m_attribTicketPanel = new MiscTicketPanel();
		m_volPanel = new VolatilityTicketPanel();
		m_comboPanel = new ComboTicketPanel();
		m_algoPanel = new AlgoPanel();
		m_scalePanel = new ScalePanel();
		
		HtmlButton transmitOrder = new HtmlButton( "Transmit Order") {
			@Override public void actionPerformed() {
				onTransmitOrder();
			}
		};
		
		HtmlButton checkMargin = new HtmlButton( "Check Margin") {
			@Override public void actionPerformed() {
				onCheckMargin();
			}
		};
		
		HtmlButton close = new HtmlButton( "Close") {
			@Override public void actionPerformed() {
				dispose();
			}
		};
		
		NewTabbedPanel tabbedPanel = new NewTabbedPanel(true);
		if (m_editContract) {
			tabbedPanel.addTab( "Contract", m_contractPanel);
		}
		tabbedPanel.addTab( "Order", m_orderPanel);
		tabbedPanel.addTab( "Misc", m_attribTicketPanel);
		tabbedPanel.addTab( "Advisor", m_advisorPanel);
		tabbedPanel.addTab( "Volatility", m_volPanel);
		if (m_contract.isCombo() ) {
			tabbedPanel.addTab( "Combo", m_comboPanel);
		}
		tabbedPanel.addTab( "Scale", m_scalePanel);
		tabbedPanel.addTab( "IB Algo", m_algoPanel);
		
		JPanel buts = new JPanel( new FlowLayout( FlowLayout.CENTER, 15, 5));
		buts.add( transmitOrder);		
		buts.add( checkMargin);
		buts.add( close);
		
		// check-margin is for new orders only
		if (m_order.orderId() != 0) {
			checkMargin.setVisible( false);
		}

		add( tabbedPanel);
		add( buts, BorderLayout.SOUTH);

		setLocation(200, 200);
		pack();
		Util.closeOnEsc( this);
	}
	
	protected void onTransmitOrder() {
		scrape();
		
		// close window right away for mods
		if (m_order.orderId() != 0) {
			dispose();
		}

		ApiDemo.INSTANCE.controller().placeOrModifyOrder( m_contract, m_order, new IOrderHandler() {
			@Override public void orderState(NewOrderState orderState) {
				ApiDemo.INSTANCE.controller().removeOrderHandler( this);
				SwingUtilities.invokeLater( new Runnable() {
					@Override public void run() {
						dispose();
					}
				});
			}
			@Override public void handle(int errorCode, final String errorMsg) {
				m_order.orderId( 0);
				SwingUtilities.invokeLater( new Runnable() {
					@Override public void run() {
						JOptionPane.showMessageDialog( TicketDlg.this, errorMsg);
					}
				});
			}
		});
	}

	protected void onCheckMargin() {
		scrape();
		
		m_order.whatIf( true);
		ApiDemo.INSTANCE.controller().placeOrModifyOrder( m_contract, m_order, new IOrderHandler() {
			@Override public void orderState(final NewOrderState orderState) {
				SwingUtilities.invokeLater( new Runnable() {
					@Override public void run() {
						displayMargin( orderState);
					}
				});
			}
			@Override public void handle(int errorCode, final String errorMsg) {
				SwingUtilities.invokeLater( new Runnable() {
					@Override public void run() {
						JOptionPane.showMessageDialog( TicketDlg.this, errorMsg);
					}
				});
			}
		});
		
		m_order.whatIf( false);
		m_order.orderId( 0);
	}

	protected void displayMargin(NewOrderState orderState) {
		String str = String.format( "Equity with loan: %s\n\nInitial margin: %s\nMaintenance margin: %s\n",
				fmt( Double.parseDouble(orderState.equityWithLoan() ) ),
				fmt( Double.parseDouble( orderState.initMargin() ) ),
				fmt( Double.parseDouble(orderState.maintMargin() ) ) );
		
		JOptionPane.showMessageDialog( this, str, "Post-Trade Margin Requirements", JOptionPane.INFORMATION_MESSAGE);
	}

	private void scrape() {
		m_order.smartComboRoutingParams().clear();
		
		if (m_editContract) {
			m_contractPanel.onOK();
		}
		m_orderPanel.onOK();
		m_advisorPanel.onOK();
		m_attribTicketPanel.onOK();
		m_volPanel.onOK();
		m_algoPanel.onOK();
		m_scalePanel.onOK();
		if (m_contract.isCombo() ) {
			m_comboPanel.onOK();
		}
	}

	class OrderPanel extends VerticalPanel {
		final TCombo<String> m_account = new TCombo<String>( ApiDemo.INSTANCE.accountList().toArray(new String[0]) );
		final TCombo<Action> m_action = new TCombo<Action>( Action.values() );
		final UpperField m_quantity = new UpperField( "100");
		final UpperField m_displaySize = new UpperField();
		final TCombo<OrderType> m_orderType = new TCombo<OrderType>( OrderType.values() ); 
		final UpperField m_lmtPrice = new UpperField( "200");
		final UpperField m_auxPrice = new UpperField();
		final TCombo<TimeInForce> m_tif = new TCombo<TimeInForce>( TimeInForce.values() );
		final JCheckBox m_nonGuaranteed = new JCheckBox();

		OrderPanel() {
			m_orderType.removeItemAt( 0); // remove None
			
			m_account.setSelectedItem( m_order.account() != null ? m_order.account() : ApiDemo.INSTANCE.accountList().get( 0) ); 
			m_action.setSelectedItem( m_order.action() );
			m_quantity.setText( m_order.totalQuantity());
			m_displaySize.setText( m_order.displaySize());
			m_orderType.setSelectedItem( m_order.orderType() );
			m_lmtPrice.setText( m_order.lmtPrice());
			m_auxPrice.setText( m_order.auxPrice());
			m_tif.setSelectedItem( m_order.tif());
			m_nonGuaranteed.setSelected( getVal( ComboParam.NonGuaranteed).equals( "1") );
			
			add( "Account", m_account);
			add( "Action", m_action);
			add( "Quantity", m_quantity);
			add( "Display size", m_displaySize);
			add( "Order type", m_orderType);
			add( "Limit price", m_lmtPrice);
			add( "Aux price", m_auxPrice);
			add( "Time-in-force", m_tif);
			if (m_contract.isCombo() ) {
				add( "Non-guaranteed", m_nonGuaranteed);
			}
		}
		
		private void onOK() {
			m_order.account( m_account.getText().toUpperCase() );
			m_order.action( m_action.getSelectedItem() );
			m_order.totalQuantity( m_quantity.getInt() );
			m_order.displaySize( m_displaySize.getInt() );
			m_order.orderType( m_orderType.getSelectedItem() );
			m_order.lmtPrice( m_lmtPrice.getDouble() );
			m_order.auxPrice( m_auxPrice.getDouble() );
			m_order.tif( m_tif.getSelectedItem() );
			if (m_contract.isCombo() ) {
				TagValue tv = new TagValue( ComboParam.NonGuaranteed.toString(), m_nonGuaranteed.isSelected() ? "1" : "0");
				m_order.smartComboRoutingParams().add( tv);
			}
		}
	}
	
	class AdvisorTicketPanel extends VerticalPanel {
		final UpperField m_faGroup = new UpperField();
		final TCombo<Method> m_faMethod = new TCombo<Method>( Method.values() );
		final UpperField m_faPercentage = new UpperField();
		final UpperField m_faProfile = new UpperField();
		
		AdvisorTicketPanel() {
			m_faGroup.setText( m_order.faGroup() );
			m_faMethod.setSelectedItem( m_order.faMethod() );
			m_faPercentage.setText( m_order.faPercentage() );
			m_faProfile.setText( m_order.faProfile() );
			
			add( "Group", m_faGroup);
			add( "Method", m_faMethod);
			add( "Percentage", m_faPercentage);
			add( Box.createVerticalStrut(10));
			add( "--or--");
			add( Box.createVerticalStrut(10));
			add( "Profile", m_faProfile);
		}
		
		void onOK() {
			m_order.faGroup( m_faGroup.getText() );
			m_order.faMethod( m_faMethod.getSelectedItem() );
			m_order.faPercentage( m_faPercentage.getText() );
			m_order.faProfile( m_faProfile.getText() );
		}
	}
	
	class MiscTicketPanel extends StackPanel {
		final UpperField m_goodAfter = new UpperField();
		final UpperField m_goodTil = new UpperField();
		final JTextField m_orderRef = new JTextField(7);
		final JTextField m_ocaGroup = new JTextField(7);
		final UpperField m_minQty = new UpperField();
		final UpperField m_percentOffset = new UpperField();
		final UpperField m_trailingStopPrice = new UpperField();
		final UpperField m_trailingPercent = new UpperField();
		final UpperField m_discretionaryAmt = new UpperField();
		final UpperField m_nbboPriceCap = new UpperField();

		final TCombo<OcaType> m_ocaType = new TCombo<OcaType>( OcaType.values() );
		final TCombo<Rule80A> m_rule80A = new TCombo<Rule80A>( Rule80A.values() );
		final TCombo<TriggerMethod> m_trigger = new TCombo<TriggerMethod>( TriggerMethod.values() );

		final TCombo<HedgeType> m_hedgeType = new TCombo<HedgeType>( HedgeType.values() );
		final UpperField m_hedgeParam = new UpperField();
		
		final JCheckBox m_blockOrder = new JCheckBox();
		final JCheckBox m_sweepToFill = new JCheckBox();
		final JCheckBox m_hidden = new JCheckBox();
		final JCheckBox m_outsideRth = new JCheckBox();
		final JCheckBox m_allOrNone = new JCheckBox();
		final JCheckBox m_overrideConstraints = new JCheckBox();
		final JCheckBox m_notHeld = new JCheckBox();
		final JCheckBox m_transmit = new JCheckBox();
		final JCheckBox m_eTradeOnly = new JCheckBox();
		final JCheckBox m_firmQuoteOnly = new JCheckBox();
		final JCheckBox m_optOutSmartRouting = new JCheckBox();
		
		

		MiscTicketPanel() {
			VerticalPanel top = new VerticalPanel();
			top.add( "Order ref", m_orderRef);
			top.add( "Min Qty", m_minQty);
			top.add( "Good after", m_goodAfter);
			top.add( "Good until", m_goodTil);
			top.add( "Rule 80A", m_rule80A);
			top.add( "Trigger method", m_trigger);
			top.add( "Percent Offset", m_percentOffset);
			top.add( "Trail order stop price", m_trailingStopPrice);
			top.add( "Trailing percent", m_trailingPercent);
			top.add("Discretionary amount", m_discretionaryAmt);
			top.add("NBBO price cap", m_nbboPriceCap);

			top.add( "OCA group and type", m_ocaGroup, m_ocaType);
			//add( "OCA type", m_ocaType);
			
			top.add( "Hedge type and param" , m_hedgeType, m_hedgeParam);
			//add( "Hedge param", m_hedgeParam);
			
			
			VerticalPanel left = new VerticalPanel();
			left.add( "Not held", m_notHeld);
			left.add( "Block order", m_blockOrder);
			left.add( "Sweep-to-fill", m_sweepToFill);
			left.add( "Hidden", m_hidden);
			left.add( "Fill outside RTH", m_outsideRth);
			left.add( "All-or-none", m_allOrNone);

			VerticalPanel right = new VerticalPanel();
			right.add( "Override constraints", m_overrideConstraints);
			right.add( "E-trade only", m_eTradeOnly);
			right.add( "Firm quote only", m_firmQuoteOnly);
			right.add( "Opt out SMART routing", m_optOutSmartRouting);
			right.add( "Transmit", m_transmit);
			
			HorzPanel checks = new HorzPanel();
			checks.add( left);
			checks.add( Box.createHorizontalGlue());
			checks.add( right);
			checks.add( Box.createHorizontalGlue());
			
			add( top);
			add( Box.createVerticalStrut(20));
			add( checks);

			m_minQty.setText( m_order.minQty() );
			m_goodAfter.setText( m_order.goodAfterTime() );
			m_goodTil.setText( m_order.goodTillDate() );
			m_orderRef.setText( m_order.orderRef() );
			m_ocaGroup.setText( m_order.ocaGroup() );
			m_ocaType.setSelectedItem( m_order.ocaType() );
			m_rule80A.setSelectedItem( m_order.rule80A() );
			m_trigger.setSelectedItem( m_order.triggerMethod() );
			m_blockOrder.setSelected( m_order.blockOrder() );
			m_sweepToFill.setSelected( m_order.sweepToFill() );
			m_hidden.setSelected( m_order.hidden() );
			m_outsideRth.setSelected( m_order.outsideRth() );
			m_allOrNone.setSelected( m_order.allOrNone() );
			m_overrideConstraints.setSelected( m_order.overridePercentageConstraints() );
			m_hedgeType.setSelectedItem( m_order.hedgeType() );
			m_hedgeParam.setText( m_order.hedgeParam() );
			m_notHeld.setSelected( m_order.notHeld() );
			m_percentOffset.setText( m_order.percentOffset() );
			m_trailingStopPrice.setText( m_order.trailStopPrice() );
			m_trailingPercent.setText( m_order.trailingPercent() );
			m_discretionaryAmt.setText( m_order.discretionaryAmt() );
			m_eTradeOnly.setSelected( m_order.eTradeOnly() );
			m_firmQuoteOnly.setSelected( m_order.firmQuoteOnly() );
			m_nbboPriceCap.setText( m_order.nbboPriceCap() );
			m_optOutSmartRouting.setSelected( m_order.optOutSmartRouting() );
			m_transmit.setSelected( true);
		}
		
		void onOK() {
			m_order.minQty( m_minQty.getInt() );
			m_order.goodAfterTime( m_goodAfter.getText() );
			m_order.goodTillDate( m_goodTil.getText() );
			m_order.orderRef( m_orderRef.getText() );
			m_order.ocaGroup( m_ocaGroup.getText() );
			m_order.ocaType( m_ocaType.getSelectedItem() );
			m_order.rule80A( m_rule80A.getSelectedItem() );
			m_order.triggerMethod( m_trigger.getSelectedItem() );
			m_order.sweepToFill( m_sweepToFill.isSelected() );
			m_order.hidden( m_hidden.isSelected() );
			m_order.outsideRth( m_outsideRth.isSelected() );
			m_order.allOrNone( m_allOrNone.isSelected() );
			m_order.overridePercentageConstraints( m_overrideConstraints.isSelected() );
			m_order.hedgeType( m_hedgeType.getSelectedItem() );
			m_order.hedgeParam( m_hedgeParam.getText() );
			m_order.notHeld( m_notHeld.isSelected() );
			m_order.percentOffset( m_percentOffset.getDouble() );
			m_order.trailStopPrice( m_trailingStopPrice.getDouble() );
			m_order.trailingPercent( m_trailingPercent.getDouble() );
			m_order.discretionaryAmt( m_discretionaryAmt.getDouble() );
			m_order.eTradeOnly( m_eTradeOnly.isSelected() );
			m_order.firmQuoteOnly( m_firmQuoteOnly.isSelected() );
			m_order.nbboPriceCap( m_nbboPriceCap.getDouble() );
			m_order.optOutSmartRouting( m_optOutSmartRouting.isSelected() );
			m_order.transmit( m_transmit.isSelected() );
		}
	}
	
	class VolatilityTicketPanel extends VerticalPanel {
		final UpperField m_volatility = new UpperField();
		final TCombo<VolatilityType> m_volatilityType = new TCombo<VolatilityType>( VolatilityType.values() );
		final JCheckBox m_continuousUpdate = new JCheckBox();
		final TCombo<ReferencePriceType> m_referencePriceType = new TCombo<ReferencePriceType>(ReferencePriceType.values());
		final TCombo<OrderType> m_deltaNeutralOrderType = new TCombo<OrderType>( OrderType.values() );
		final UpperField m_deltaNeutralAuxPrice = new UpperField();
		final UpperField m_deltaNeutralConId = new UpperField();
		final UpperField m_upper = new UpperField();
		final UpperField m_lower = new UpperField();
		
		VolatilityTicketPanel() {
			add( "Volatility", m_volatility, m_volatilityType);
			add( "Continuously update price", m_continuousUpdate);
			add( "Option reference price", m_referencePriceType);
			add( "Hedge order type", m_deltaNeutralOrderType);
			add( "Hedge order aux price", m_deltaNeutralAuxPrice);
			add( "Hedge contract conid", m_deltaNeutralConId);
			add( "Stock range - upper", m_upper);
			add( "Stock range - lower", m_lower);
			
			m_volatility.setText( m_order.volatility() );
			m_volatilityType.setSelectedItem( m_order.volatilityType() );
			m_continuousUpdate.setSelected( m_order.continuousUpdate() );
			m_referencePriceType.setSelectedItem( m_order.referencePriceType() );
			m_deltaNeutralOrderType.setSelectedItem( m_order.deltaNeutralOrderType() );
			m_deltaNeutralAuxPrice.setText( m_order.deltaNeutralAuxPrice() );
			m_deltaNeutralConId.setText( m_order.deltaNeutralConId() );
			m_upper.setText( m_order.stockRangeUpper() );
			m_lower.setText( m_order.stockRangeLower() );
		}
		
		void onOK() {
			m_order.volatility( m_volatility.getDouble() );
			m_order.volatilityType( m_volatilityType.getSelectedItem() );
			m_order.continuousUpdate( m_continuousUpdate.isSelected() );
			m_order.referencePriceType( m_referencePriceType.getSelectedItem() );
			m_order.deltaNeutralOrderType( m_deltaNeutralOrderType.getSelectedItem() );
			m_order.deltaNeutralAuxPrice( m_deltaNeutralAuxPrice.getDouble() );
			m_order.deltaNeutralConId( m_deltaNeutralConId.getInt() );
			m_order.stockRangeUpper( m_upper.getDouble() );
			m_order.stockRangeLower( m_lower.getDouble() );
		}
	}
	
	/** This panel edits all ComboParam values except for Non-Guaranteed. 
	 *  That one goes on main panel because it applies to all combo orders. */
	class ComboTicketPanel extends VerticalPanel {
		final UpperField[] m_fields = new UpperField[ComboParam.values().length];
		
		ComboTicketPanel() {
			for (ComboParam param : ComboParam.values() ) {
				if (param == ComboParam.NonGuaranteed) {
					continue;
				}
				UpperField field = new UpperField();
				m_fields[param.ordinal()] = field;
				add( param.toString(), field);
				field.setText( getVal( param) );
			}
		}
		
		void onOK() {
			for (ComboParam param : ComboParam.values() ) {
				if (param == ComboParam.NonGuaranteed) {
					continue;
				}
				UpperField field = m_fields[param.ordinal()];
				String val = field.getText();
				if (val != null && val.length() > 0) {
					TagValue tv = new TagValue( param.toString(), val);
					m_order.smartComboRoutingParams().add( tv);
				}
			}
		}
	}
	
	class AlgoPanel extends VerticalPanel {
		final TCombo<AlgoStrategy> m_strategy = new TCombo<AlgoStrategy>( AlgoStrategy.values() );
		final UpperField[] m_params = new UpperField[AlgoParam.values().length]; 

		AlgoPanel() {
			add( "Algo strategy", m_strategy);
			
			for (AlgoParam param : AlgoParam.values() ) {
				int i = param.ordinal();
				m_params[i] = new UpperField(11);
				add( param.toString(), m_params[param.ordinal()]);
			}
			
			m_strategy.setSelectedItem( m_order.algoStrategy() );
			
			if (m_order.algoParams() != null) {
				for (TagValue tagVal : m_order.algoParams() ) {
					AlgoParam param = AlgoParam.valueOf( tagVal.m_tag);
					m_params[param.ordinal()].setText( tagVal.m_value);
				}
			}

			m_strategy.addActionListener( new ActionListener() {
				@Override public void actionPerformed(ActionEvent e) {
					onSelChanged();
				}
			});
			
			onSelChanged();
		}
		
		void onSelChanged() {
			for (int i = 0; i < m_params.length; i++) {
				m_params[i].setEnabled( false);
			}
			
			AlgoStrategy strategy = m_strategy.getSelectedItem();
			if (strategy != null) {
				for (AlgoParam param : strategy.params() ) {
					m_params[param.ordinal()].setEnabled( true);
				}
			}
		}

		void onOK() {
			m_order.algoStrategy( m_strategy.getSelectedItem() );
			
			m_order.algoParams().clear();
			for (AlgoParam param : AlgoParam.values() ) {
				String val = m_params[param.ordinal()].getText();
				if (val != null && val.length() > 0) {
					m_order.algoParams().add( new TagValue( param.toString(), val) );
				}
			}
		}
	}

	class ScalePanel extends VerticalPanel {
		UpperField m_initLevelSize = new UpperField();
		UpperField m_subsLevelSize = new UpperField();
		UpperField m_priceIncrement = new UpperField();
		UpperField m_priceAdjustValue = new UpperField();
		UpperField m_priceAdjustInterval = new UpperField();
		UpperField m_profitOffset = new UpperField();
		JCheckBox m_autoReset = new JCheckBox();
		UpperField m_initPosition = new UpperField();
		UpperField m_initFillQty = new UpperField();
		JCheckBox m_randomPercent = new JCheckBox();
		UpperField m_table = new UpperField();

		ScalePanel() {
			m_initLevelSize.setText( m_order.scaleInitLevelSize() );
			m_subsLevelSize.setText( m_order.scaleSubsLevelSize() );
			m_priceIncrement.setText( m_order.scalePriceIncrement() );
			m_priceAdjustValue.setText( m_order.scalePriceAdjustValue() );
			m_priceAdjustInterval.setText( m_order.scalePriceAdjustInterval() );
			m_profitOffset.setText( m_order.scaleProfitOffset() );
			m_autoReset.setSelected( m_order.scaleAutoReset() );
			m_initPosition.setText( m_order.scaleInitPosition() );
			m_initFillQty.setText( m_order.scaleInitFillQty() );
			m_randomPercent.setSelected( m_order.scaleRandomPercent() );
			m_table.setText( m_order.scaleTable() );

			add( "Initial comp size", m_initLevelSize);
			add( "Subsequent comp size", m_subsLevelSize);
			add( "Randomize size", m_randomPercent);
			add( Box.createVerticalStrut( 10) );
			add( "Price increment", m_priceIncrement);
			add( "Profit offset", m_profitOffset);
			add( "Auto-reset", m_autoReset);
			add( Box.createVerticalStrut( 10) );
			add( "Initial position", m_initPosition);
			add( "Filled init comp size", m_initFillQty);
			add( Box.createVerticalStrut( 10) );
			add( "Increase price by", m_priceAdjustValue, new JLabel( "every"), m_priceAdjustInterval, new JLabel( "seconds") );
			add( Box.createVerticalStrut( 10) );
			add( "Manual table", m_table);
		}
	
		void onOK() {
			m_order.scaleInitLevelSize( m_initLevelSize.getInt()); 
			m_order.scaleSubsLevelSize( m_subsLevelSize.getInt()); 
			m_order.scalePriceIncrement( m_priceIncrement.getDouble()); 
			m_order.scalePriceAdjustValue( m_priceAdjustValue.getDouble()); 
			m_order.scalePriceAdjustInterval( m_priceAdjustInterval.getInt()); 
			m_order.scaleProfitOffset( m_profitOffset.getDouble()); 
			m_order.scaleAutoReset( m_autoReset.isSelected()); 
			m_order.scaleInitPosition( m_initPosition.getInt()); 
			m_order.scaleInitFillQty( m_initFillQty.getInt()); 
			m_order.scaleRandomPercent( m_randomPercent.isSelected());
			m_order.scaleTable( m_table.getText() );
		}
	}

	private String getVal(ComboParam param) {
		if (m_order.smartComboRoutingParams() != null) {
			for (TagValue tv : m_order.smartComboRoutingParams() ) {
				if (tv.m_tag.equals( param.toString() ) ) {
					return tv.m_value; 
				}
			}
		}
		return "";
	}
}
