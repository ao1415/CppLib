using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CodeCompressor
{
    public partial class CodeForm : Form
    {
        public CodeForm(string code = "")
        {
            InitializeComponent();

            richTextBoxCode.Text = code;
        }
    }
}
