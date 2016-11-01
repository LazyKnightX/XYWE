﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using XYBase;

namespace XYWE
{
    public partial class FormXYWE : Form
    {
        static FormXYWE Last = null;

        public FormXYWE()
        {
            InitializeComponent();
            Last = this;
        }

        void FormXYWE_Load(object sender, EventArgs e)
        {
            LlVersion.Text = XYInfo.Version;
            XYTip.UpdateTipAsync();
            XYVersion.VerifyObsoleteAsync(ObsoleteAction);
        }
        void ObsoleteAction()
        {
            LlVersion.Text = LlVersion.Text + "(旧)";
            LlVersion.LinkColor = System.Drawing.Color.Red;
        }

        void LlVersion_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            XYProcess.Website.StartOfficial();
        }

        void BtnStartXYWE_Click(object sender, EventArgs e)
        {
            XYTip.Refresh();
            XYFile.SyncDirectory(XYPath.Dir.SourceJass, XYPath.Dir.EditorJass);
            XYFile.SyncDirectory(XYPath.Dir.SourceMpq, XYPath.Dir.EditorShareMpq);
            XYFile.SyncDirectory(XYPath.Dir.SourcePlugin, XYPath.Dir.EditorPlugin);
            XYFile.SyncDirectory(XYPath.Dir.SourceScript, XYPath.Dir.EditorShareScript);
            XYFile.Compile(XYPath.Dir.SourceUi + @"\base\TriggerData", XYPath.Dir.EditorShareMpq + @"\base\TriggerData.txt");
            XYFile.Compile(XYPath.Dir.SourceUi + @"\base\TriggerStrings", XYPath.Dir.EditorShareMpq + @"\base\TriggerStrings.txt");
            XYFile.Compile(XYPath.Dir.SourceUi + @"\base\WorldEditStrings", XYPath.Dir.EditorShareMpq + @"\base\WorldEditStrings.txt");
            // XYProcess.Application.StartXYWE();
            MessageBox.Show("Start");
        }

        void BtnXYCodeLibraryManager_Click(object sender, EventArgs e)
        {
            XYProcess.Application.StartXYCodeLibraryManager();
        }

        void BtnXYUILibraryManager_Click(object sender, EventArgs e)
        {
            XYProcess.Application.StartXYMpqLibraryManager();
        }

        private void BtnXYTextColorMaker_Click(object sender, EventArgs e)
        {
            XYProcess.Application.StartXYTextColorMaker();
        }

        private void BtnXYTriggerSyntaxHighlighter_Click(object sender, EventArgs e)
        {
            XYProcess.Application.StartXYTriggerSyntaxHighlighter();
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            XYProcess.Website.StartCommunity();
        }
    }
}
