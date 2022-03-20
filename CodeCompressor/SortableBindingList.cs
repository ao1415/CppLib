using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CodeCompressor
{
    internal class SortableBindingList<T> : BindingList<T>
    {
        public void Sort()
        {
            if (Items.Count > 1)
            {
                ((List<T>)Items).Sort();
                OnListChanged(new ListChangedEventArgs(ListChangedType.Reset, -1));
            }
        }
    }
}
