namespace CodeCompressor
{
    partial class MainForm
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.Windows.Forms.GroupBox groupBox1;
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.listBoxExclusion = new System.Windows.Forms.ListBox();
            this.listBoxInclude = new System.Windows.Forms.ListBox();
            this.buttonAllExclusion = new System.Windows.Forms.Button();
            this.buttonExclusion = new System.Windows.Forms.Button();
            this.buttonInclude = new System.Windows.Forms.Button();
            this.buttonAllInclude = new System.Windows.Forms.Button();
            this.textBoxRootPath = new System.Windows.Forms.TextBox();
            this.buttonRootFolder = new System.Windows.Forms.Button();
            this.buttonGeneration = new System.Windows.Forms.Button();
            groupBox1 = new System.Windows.Forms.GroupBox();
            groupBox1.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            groupBox1.Controls.Add(this.tableLayoutPanel1);
            groupBox1.Location = new System.Drawing.Point(12, 49);
            groupBox1.Name = "groupBox1";
            groupBox1.Size = new System.Drawing.Size(1005, 569);
            groupBox1.TabIndex = 8;
            groupBox1.TabStop = false;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 3;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 68F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Controls.Add(this.listBoxExclusion, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.listBoxInclude, 2, 0);
            this.tableLayoutPanel1.Controls.Add(this.buttonAllExclusion, 1, 5);
            this.tableLayoutPanel1.Controls.Add(this.buttonExclusion, 1, 4);
            this.tableLayoutPanel1.Controls.Add(this.buttonInclude, 1, 2);
            this.tableLayoutPanel1.Controls.Add(this.buttonAllInclude, 1, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(3, 25);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 7;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 38F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 38F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 38F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 38F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(999, 541);
            this.tableLayoutPanel1.TabIndex = 2;
            // 
            // listBoxExclusion
            // 
            this.listBoxExclusion.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listBoxExclusion.FormattingEnabled = true;
            this.listBoxExclusion.HorizontalScrollbar = true;
            this.listBoxExclusion.ItemHeight = 21;
            this.listBoxExclusion.Location = new System.Drawing.Point(4, 4);
            this.listBoxExclusion.Margin = new System.Windows.Forms.Padding(4);
            this.listBoxExclusion.Name = "listBoxExclusion";
            this.tableLayoutPanel1.SetRowSpan(this.listBoxExclusion, 7);
            this.listBoxExclusion.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.listBoxExclusion.Size = new System.Drawing.Size(457, 533);
            this.listBoxExclusion.TabIndex = 0;
            // 
            // listBoxInclude
            // 
            this.listBoxInclude.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listBoxInclude.FormattingEnabled = true;
            this.listBoxInclude.HorizontalScrollbar = true;
            this.listBoxInclude.ItemHeight = 21;
            this.listBoxInclude.Location = new System.Drawing.Point(537, 4);
            this.listBoxInclude.Margin = new System.Windows.Forms.Padding(4);
            this.listBoxInclude.Name = "listBoxInclude";
            this.tableLayoutPanel1.SetRowSpan(this.listBoxInclude, 7);
            this.listBoxInclude.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.listBoxInclude.Size = new System.Drawing.Size(458, 533);
            this.listBoxInclude.TabIndex = 5;
            // 
            // buttonAllExclusion
            // 
            this.buttonAllExclusion.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonAllExclusion.Location = new System.Drawing.Point(469, 376);
            this.buttonAllExclusion.Margin = new System.Windows.Forms.Padding(4);
            this.buttonAllExclusion.Name = "buttonAllExclusion";
            this.buttonAllExclusion.Size = new System.Drawing.Size(60, 30);
            this.buttonAllExclusion.TabIndex = 4;
            this.buttonAllExclusion.Text = "<<";
            this.buttonAllExclusion.UseVisualStyleBackColor = true;
            this.buttonAllExclusion.Click += new System.EventHandler(this.ButtonAllExclusion_Click);
            // 
            // buttonExclusion
            // 
            this.buttonExclusion.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonExclusion.Location = new System.Drawing.Point(469, 338);
            this.buttonExclusion.Margin = new System.Windows.Forms.Padding(4);
            this.buttonExclusion.Name = "buttonExclusion";
            this.buttonExclusion.Size = new System.Drawing.Size(60, 30);
            this.buttonExclusion.TabIndex = 3;
            this.buttonExclusion.Text = "<";
            this.buttonExclusion.UseVisualStyleBackColor = true;
            this.buttonExclusion.Click += new System.EventHandler(this.ButtonExclusion_Click);
            // 
            // buttonInclude
            // 
            this.buttonInclude.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonInclude.Location = new System.Drawing.Point(469, 171);
            this.buttonInclude.Margin = new System.Windows.Forms.Padding(4);
            this.buttonInclude.Name = "buttonInclude";
            this.buttonInclude.Size = new System.Drawing.Size(60, 30);
            this.buttonInclude.TabIndex = 2;
            this.buttonInclude.Text = ">";
            this.buttonInclude.UseVisualStyleBackColor = true;
            this.buttonInclude.Click += new System.EventHandler(this.ButtonInclude_Click);
            // 
            // buttonAllInclude
            // 
            this.buttonAllInclude.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonAllInclude.Location = new System.Drawing.Point(469, 133);
            this.buttonAllInclude.Margin = new System.Windows.Forms.Padding(4);
            this.buttonAllInclude.Name = "buttonAllInclude";
            this.buttonAllInclude.Size = new System.Drawing.Size(60, 30);
            this.buttonAllInclude.TabIndex = 1;
            this.buttonAllInclude.Text = ">>";
            this.buttonAllInclude.UseVisualStyleBackColor = true;
            this.buttonAllInclude.Click += new System.EventHandler(this.ButtonAllInclude_Click);
            // 
            // textBoxRootPath
            // 
            this.textBoxRootPath.Font = new System.Drawing.Font("Yu Gothic UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.textBoxRootPath.Location = new System.Drawing.Point(12, 13);
            this.textBoxRootPath.Margin = new System.Windows.Forms.Padding(4);
            this.textBoxRootPath.Name = "textBoxRootPath";
            this.textBoxRootPath.ReadOnly = true;
            this.textBoxRootPath.Size = new System.Drawing.Size(410, 29);
            this.textBoxRootPath.TabIndex = 0;
            this.textBoxRootPath.TextChanged += new System.EventHandler(this.TextBoxRootPath_TextChanged);
            // 
            // buttonRootFolder
            // 
            this.buttonRootFolder.Location = new System.Drawing.Point(430, 13);
            this.buttonRootFolder.Margin = new System.Windows.Forms.Padding(4);
            this.buttonRootFolder.Name = "buttonRootFolder";
            this.buttonRootFolder.Size = new System.Drawing.Size(64, 29);
            this.buttonRootFolder.TabIndex = 1;
            this.buttonRootFolder.Text = "選択";
            this.buttonRootFolder.UseVisualStyleBackColor = true;
            this.buttonRootFolder.Click += new System.EventHandler(this.ButtonRootFolder_Click);
            // 
            // buttonGeneration
            // 
            this.buttonGeneration.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonGeneration.Location = new System.Drawing.Point(921, 12);
            this.buttonGeneration.Name = "buttonGeneration";
            this.buttonGeneration.Size = new System.Drawing.Size(96, 29);
            this.buttonGeneration.TabIndex = 9;
            this.buttonGeneration.Text = "生成";
            this.buttonGeneration.UseVisualStyleBackColor = true;
            this.buttonGeneration.Click += new System.EventHandler(this.ButtonGeneration_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 21F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1029, 630);
            this.Controls.Add(this.buttonGeneration);
            this.Controls.Add(groupBox1);
            this.Controls.Add(this.buttonRootFolder);
            this.Controls.Add(this.textBoxRootPath);
            this.Font = new System.Drawing.Font("Yu Gothic UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "MainForm";
            this.Text = "Form1";
            groupBox1.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private ListBox listBoxExclusion;
        private Button buttonAllInclude;
        private Button buttonInclude;
        private Button buttonExclusion;
        private Button buttonAllExclusion;
        private ListBox listBoxInclude;
        private TextBox textBoxRootPath;
        private Button buttonRootFolder;
        private TableLayoutPanel tableLayoutPanel1;
        private Button buttonGeneration;
    }
}