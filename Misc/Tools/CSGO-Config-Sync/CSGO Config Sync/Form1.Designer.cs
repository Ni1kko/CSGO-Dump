namespace CSGO_Config_Sync
{
    partial class Main
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.txtbox_steampath = new System.Windows.Forms.TextBox();
            this.lbl_steamPath = new System.Windows.Forms.Label();
            this.txt_steamID = new System.Windows.Forms.TextBox();
            this.lbl_steamid = new System.Windows.Forms.Label();
            this.btn_sync = new System.Windows.Forms.Button();
            this.lbl_syncwarning = new System.Windows.Forms.Label();
            this.lbl_status = new System.Windows.Forms.Label();
            this.list_steamUsernames = new System.Windows.Forms.ListBox();
            this.SuspendLayout();
            // 
            // txtbox_steampath
            // 
            this.txtbox_steampath.Location = new System.Drawing.Point(12, 39);
            this.txtbox_steampath.Name = "txtbox_steampath";
            this.txtbox_steampath.Size = new System.Drawing.Size(547, 20);
            this.txtbox_steampath.TabIndex = 0;
            this.txtbox_steampath.TextChanged += new System.EventHandler(this.TextBox1_TextChanged);
            // 
            // lbl_steamPath
            // 
            this.lbl_steamPath.AutoSize = true;
            this.lbl_steamPath.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_steamPath.Location = new System.Drawing.Point(9, 21);
            this.lbl_steamPath.Name = "lbl_steamPath";
            this.lbl_steamPath.Size = new System.Drawing.Size(74, 15);
            this.lbl_steamPath.TabIndex = 1;
            this.lbl_steamPath.Text = "Steam Path:";
            // 
            // txt_steamID
            // 
            this.txt_steamID.Location = new System.Drawing.Point(12, 100);
            this.txt_steamID.Name = "txt_steamID";
            this.txt_steamID.Size = new System.Drawing.Size(547, 20);
            this.txt_steamID.TabIndex = 2;
            this.txt_steamID.TextChanged += new System.EventHandler(this.txt_steamID_TextChanged);
            // 
            // lbl_steamid
            // 
            this.lbl_steamid.AutoSize = true;
            this.lbl_steamid.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_steamid.Location = new System.Drawing.Point(9, 82);
            this.lbl_steamid.Name = "lbl_steamid";
            this.lbl_steamid.Size = new System.Drawing.Size(203, 15);
            this.lbl_steamid.TabIndex = 3;
            this.lbl_steamid.Text = "Steam ID3 (example: 1028635752):";
            // 
            // btn_sync
            // 
            this.btn_sync.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btn_sync.Location = new System.Drawing.Point(13, 306);
            this.btn_sync.Name = "btn_sync";
            this.btn_sync.Size = new System.Drawing.Size(546, 58);
            this.btn_sync.TabIndex = 4;
            this.btn_sync.Text = "Sync Config";
            this.btn_sync.UseVisualStyleBackColor = true;
            this.btn_sync.Click += new System.EventHandler(this.btn_sync_Click);
            // 
            // lbl_syncwarning
            // 
            this.lbl_syncwarning.AutoSize = true;
            this.lbl_syncwarning.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbl_syncwarning.Location = new System.Drawing.Point(10, 288);
            this.lbl_syncwarning.Name = "lbl_syncwarning";
            this.lbl_syncwarning.Size = new System.Drawing.Size(491, 15);
            this.lbl_syncwarning.TabIndex = 5;
            this.lbl_syncwarning.Text = "Warning: This will REPLACE all CS:GO configs from all other steam accounts ( loca" +
    "l only )";
            // 
            // lbl_status
            // 
            this.lbl_status.AutoSize = true;
            this.lbl_status.Location = new System.Drawing.Point(9, 275);
            this.lbl_status.Name = "lbl_status";
            this.lbl_status.Size = new System.Drawing.Size(60, 13);
            this.lbl_status.TabIndex = 6;
            this.lbl_status.Text = "Status: Idle";
            // 
            // list_steamUsernames
            // 
            this.list_steamUsernames.FormattingEnabled = true;
            this.list_steamUsernames.Location = new System.Drawing.Point(12, 127);
            this.list_steamUsernames.Name = "list_steamUsernames";
            this.list_steamUsernames.Size = new System.Drawing.Size(547, 147);
            this.list_steamUsernames.TabIndex = 7;
            this.list_steamUsernames.SelectedIndexChanged += new System.EventHandler(this.list_steamUsernames_SelectedIndexChanged);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(571, 376);
            this.Controls.Add(this.list_steamUsernames);
            this.Controls.Add(this.lbl_status);
            this.Controls.Add(this.lbl_syncwarning);
            this.Controls.Add(this.btn_sync);
            this.Controls.Add(this.lbl_steamid);
            this.Controls.Add(this.txt_steamID);
            this.Controls.Add(this.lbl_steamPath);
            this.Controls.Add(this.txtbox_steampath);
            this.Name = "Main";
            this.ShowIcon = false;
            this.Text = "CSGO Config Sync";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txtbox_steampath;
        private System.Windows.Forms.Label lbl_steamPath;
        private System.Windows.Forms.TextBox txt_steamID;
        private System.Windows.Forms.Label lbl_steamid;
        private System.Windows.Forms.Button btn_sync;
        private System.Windows.Forms.Label lbl_syncwarning;
        private System.Windows.Forms.Label lbl_status;
        private System.Windows.Forms.ListBox list_steamUsernames;
    }
}

