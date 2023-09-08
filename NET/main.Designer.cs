
namespace ConnectedNetwork
{
    partial class fmMain
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
            this.btGetInfo = new System.Windows.Forms.Button();
            this.lbInfo = new System.Windows.Forms.ListBox();
            this.SuspendLayout();
            // 
            // btGetInfo
            // 
            this.btGetInfo.Location = new System.Drawing.Point(12, 12);
            this.btGetInfo.Name = "btGetInfo";
            this.btGetInfo.Size = new System.Drawing.Size(121, 23);
            this.btGetInfo.TabIndex = 0;
            this.btGetInfo.Text = "Get information";
            this.btGetInfo.UseVisualStyleBackColor = true;
            this.btGetInfo.Click += new System.EventHandler(this.btGetInfo_Click);
            // 
            // lbInfo
            // 
            this.lbInfo.FormattingEnabled = true;
            this.lbInfo.Location = new System.Drawing.Point(13, 41);
            this.lbInfo.Name = "lbInfo";
            this.lbInfo.Size = new System.Drawing.Size(775, 394);
            this.lbInfo.TabIndex = 1;
            // 
            // fmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.lbInfo);
            this.Controls.Add(this.btGetInfo);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "fmMain";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Demo shows how to identify Connected WiFi Network";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btGetInfo;
        private System.Windows.Forms.ListBox lbInfo;
    }
}

