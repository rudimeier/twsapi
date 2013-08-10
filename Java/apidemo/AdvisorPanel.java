/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package apidemo;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.util.ArrayList;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.DefaultCellEditor;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.border.EmptyBorder;
import javax.swing.border.TitledBorder;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableCellEditor;


import apidemo.util.HtmlButton;
import apidemo.util.TCombo;
import apidemo.util.VerticalPanel;
import apidemo.util.NewTabbedPanel.NewTabPanel;

import com.ib.controller.Alias;
import com.ib.controller.Group;
import com.ib.controller.Profile;
import com.ib.controller.ApiController.IAdvisorHandler;
import com.ib.controller.Profile.Type;
import com.ib.controller.Types.FADataType;
import com.ib.controller.Types.Method;

public class AdvisorPanel extends NewTabPanel implements IAdvisorHandler {
	static DefaultCellEditor DEF_CELL_EDITOR = new DefaultCellEditor( new JTextField() );
	static {
		DEF_CELL_EDITOR.setClickCountToStart( 1);
	}
	
	private final GroupModel m_groupModel = new GroupModel();
	private final ProfileModel m_profileModel = new ProfileModel();
	private final AliasModel m_aliasModel = new AliasModel();
	
	private final JTable m_groupTable = new JTable( m_groupModel) {
		public TableCellEditor getCellEditor(int row, int col) {
			return m_groupModel.getCellEditor(row, col);
		}
	};

	private final JTable m_profileTable = new JTable( m_profileModel) {
		public TableCellEditor getCellEditor(int row, int col) {
			return m_profileModel.getCellEditor(row, col);
		}
	};

	private final JTable m_aliasTable = new JTable( m_aliasModel);

	AdvisorPanel() {
		JPanel mainPanel = new JPanel();
		mainPanel.setLayout( new BoxLayout( mainPanel, BoxLayout.Y_AXIS));
		mainPanel.setBorder( new EmptyBorder( 0, 10, 0, 0) );
		mainPanel.add( new GroupsPanel() );
		mainPanel.add( Box.createVerticalStrut(10));
		mainPanel.add( new ProfilesPanel() );
		
		JScrollPane aliasScroll = new JScrollPane( m_aliasTable);
		aliasScroll.setBorder( new TitledBorder( "Aliases"));
		aliasScroll.setPreferredSize(new Dimension( 300, 2000));
		
		setLayout( new BorderLayout() );
		add( aliasScroll, BorderLayout.WEST);
		add( mainPanel);
	}

	/** Called when the tab is first visited. */
	@Override public void activated() {
		ApiDemo.INSTANCE.controller().reqAdvisorData( FADataType.GROUPS, this);
		ApiDemo.INSTANCE.controller().reqAdvisorData( FADataType.PROFILES, this );
		ApiDemo.INSTANCE.controller().reqAdvisorData( FADataType.ALIASES, this );
	}
	
	/** Called when the tab is closed by clicking the X. */
	@Override public void closed() {
	}

	public void groups(ArrayList<Group> groups) {
		m_groupModel.update( groups);
	}

	public void profiles(ArrayList<Profile> profiles) {
		m_profileModel.update( profiles);
	}

	public void aliases(ArrayList<Alias> aliases) {
		m_aliasModel.update( aliases);
	}
	
	private class AliasModel extends AbstractTableModel {
		ArrayList<Alias> m_list = new ArrayList<Alias>();
		
		@Override public int getRowCount() {
			return m_list.size();
		}

		public void update(ArrayList<Alias> aliases) {
			m_list.clear();
			m_list.addAll( aliases);
			fireTableDataChanged();
		}

		@Override public int getColumnCount() {
			return 2;
		}

		@Override public String getColumnName(int col) {
			switch( col) {
				case 0: return "Account";
				case 1: return "Alias";
				default: return null;
			}
		}
		
		@Override public Object getValueAt(int rowIn, int col) {
			Alias row = m_list.get( rowIn);
			switch( col) {
				case 0: return row.account();
				case 1: return row.alias();
				default: return null;
			}
		}
	}
	
	private class GroupsPanel extends JPanel {
		GroupsPanel() {
			JScrollPane groupScroll = new JScrollPane( m_groupTable);
			groupScroll.setBorder( new TitledBorder( "Groups"));
			
			HtmlButton create = new HtmlButton( "Create Group") {
				@Override protected void actionPerformed() {
					onCreateGroup();
				}
			};

			HtmlButton update = new HtmlButton( "Update") {
				@Override protected void actionPerformed() {
					onTransmit();
				}
			};
			
			JPanel buts = new VerticalPanel();
			buts.add( create);
			buts.add( update);

			setLayout( new BorderLayout() );
			add( groupScroll);
			add( buts, BorderLayout.EAST);
		}

		protected void onCreateGroup() {
			String name = JOptionPane.showInputDialog( this, "Enter group name");
			if (name != null) {
				m_groupModel.add( name);
			}
		}

		protected void onTransmit() {
			int rc = JOptionPane.showConfirmDialog( this, "This will replace all Groups in TWS with the ones shown here.\nAre you sure you want to do that?", "Confirm", JOptionPane.YES_NO_OPTION);
			if (rc == 0) {
				m_groupModel.transmit();
				JOptionPane.showMessageDialog(this, "The groups have been updated");
			}
		}
	}
	
	private static class GroupModel extends AbstractTableModel {
		TCombo<Method> combo = new TCombo<Method>(Method.values());
		DefaultCellEditor EDITOR = new DefaultCellEditor( combo);
		ArrayList<Group> m_groups = new ArrayList<Group>();
		
		GroupModel() {		
			EDITOR.setClickCountToStart( 1);
		}
		
		void update(ArrayList<Group> groups) {
			m_groups.clear();
			m_groups.addAll( groups);
			fireTableDataChanged();
		}

		void add(String name) {
			Group group = new Group();
			group.name( name);
			m_groups.add( group);
			fireTableDataChanged();
		}

		public void transmit() {
			ApiDemo.INSTANCE.controller().updateGroups(m_groups);
		}

		@Override public int getRowCount() {
			return m_groups.size();
		}

		@Override public int getColumnCount() {
			return 3;
		}

		@Override public String getColumnName(int col) {
			switch( col) {
				case 0: return "Name";
				case 1: return "Default Method";
				case 2: return "Accounts";
				default: return null;
			}
		}
		
		@Override public Object getValueAt(int rowIn, int col) {
			Group row = m_groups.get( rowIn);
			switch( col) {
				case 0: return row.name();
				case 1: return row.defaultMethod();
				case 2: return row.accounts().toString().substring( 1, row.accounts().toString().length() - 1);
				default: return null;
			}
		}
		
		@Override public boolean isCellEditable(int rowIndex, int col) {
			return true;
		}
		
		public TableCellEditor getCellEditor(int row, int col) {
			return col == 1 ? EDITOR : DEF_CELL_EDITOR;
		}
		
		@Override public void setValueAt(Object val, int rowIn, int col) {
			Group row = m_groups.get( rowIn);
			switch( col) {
				case 0: row.name( (String)val); break;
				case 1: row.defaultMethod( (Method)val); break;
				case 2: row.setAllAccounts( (String)val); break;
			}
		}
	}
	
	private class ProfilesPanel extends JPanel {
		ProfilesPanel() {
			JScrollPane profileScroll = new JScrollPane( m_profileTable);
			profileScroll.setBorder( new TitledBorder( "Profiles"));

			HtmlButton create = new HtmlButton( "Create Profile") {
				@Override protected void actionPerformed() {
					onCreateProfile();
				}
			};

			HtmlButton update = new HtmlButton( "Update") {
				@Override protected void actionPerformed() {
					onTransmit();
				}
			};

			JPanel buts = new VerticalPanel();
			buts.add( create);
			buts.add( update);

			setLayout( new BorderLayout() );
			add( profileScroll);
			add( buts, BorderLayout.EAST);
		}

		void onCreateProfile() {
			String name = JOptionPane.showInputDialog( this, "Enter profile name");
			if (name != null) {
				m_profileModel.add( name);
			}
		}

		void onTransmit() {
			int rc = JOptionPane.showConfirmDialog( this, "This will replace all Profiles in TWS with the ones shown here.\nAre you sure you want to do that?", "Confirm", JOptionPane.YES_NO_OPTION);
			if (rc == 0) {
				m_profileModel.transmit();
				JOptionPane.showMessageDialog(this, "The Profiles have been updated");
			}
		}
	}
	
	private static class ProfileModel extends AbstractTableModel {
		TCombo<Type> combo = new TCombo<Type>(Type.values());
		DefaultCellEditor EDITOR = new DefaultCellEditor( combo);
		ArrayList<Profile> m_profiles = new ArrayList<Profile>();
		
		ProfileModel() {		
			EDITOR.setClickCountToStart( 1);
			combo.removeItemAt( 0);
		}
		
		public void update(ArrayList<Profile> profiles) {
			m_profiles.clear();
			m_profiles.addAll( profiles);
			fireTableDataChanged();
		}

		public void add(String name) {
			Profile profile = new Profile();
			profile.name( name);
			m_profiles.add( profile);
			fireTableDataChanged();
		}

		public void transmit() {
			ApiDemo.INSTANCE.controller().updateProfiles( m_profiles);
		}

		@Override public int getRowCount() {
			return m_profiles.size();
		}

		@Override public int getColumnCount() {
			return 3;
		}

		@Override public String getColumnName(int col) {
			switch( col) {
				case 0: return "Name";
				case 1: return "Type";
				case 2: return "Allocations";
				default: return null;
			}
		}
		
		@Override public Object getValueAt(int rowIn, int col) {
			Profile row = m_profiles.get( rowIn);
			switch( col) {
				case 0: return row.name();
				case 1: return row.type();
				case 2: return row.allocations().toString().substring( 1, row.allocations().toString().length() - 1);
				default: return null;
			}
		}
		
		@Override public boolean isCellEditable(int rowIndex, int columnIndex) {
			return true;
		}
		
		public TableCellEditor getCellEditor(int row, int col) {
			return col == 1 ? EDITOR : DEF_CELL_EDITOR;
		}

		@Override public void setValueAt(Object val, int rowIn, int col) {
			Profile row = m_profiles.get( rowIn);
			switch( col) {
				case 0: row.name( (String)val); break;
				case 1: row.type( (Type)val); break;
				case 2: row.setAllocations( (String)val); break;
			}
		}
	}
}
