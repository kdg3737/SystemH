using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace SharpTest {
    public class ConcurrentStack<T> {
        private class Node {
            internal readonly T m_value; // Value of the node.
            internal Node m_next; // Next pointer.
            /// <summary>
            /// Constructs a new node with the specified value and no next node.
            /// </summary>
            /// <param name="value">The value of the node.</param>
            internal Node(T value) {
                m_value = value;
                m_next = null;
            }
        }

        private volatile Node m_head;
        private const int BACKOFF_MAX_YIELDS = 8;
        public int Count {
            // Counts the number of entries in the stack. This is an O(n) operation. The answer may be out
            // of date before returning, but guarantees to return a count that was once valid. Conceptually,
            // the implementation snaps a copy of the list and then counts the entries, though physically
            // this is not what actually happens.
            get {
                int count = 0;

                // Just whip through the list and tally up the number of nodes. We rely on the fact that
                // node next pointers are immutable after being enqueued for the first time, even as
                // they are being dequeued. If we ever changed this (e.g. to pool nodes somehow),
                // we'd need to revisit this implementation.

                for (Node curr = m_head; curr != null; curr = curr.m_next) {
                    count++; //we don't handle overflow, to be consistent with existing generic collection types in CLR
                }

                return count;
            }
        }

        public void Push(T item) {
            // Pushes a node onto the front of the stack thread-safely. Internally, this simply
            // swaps the current head pointer using a (thread safe) CAS operation to accomplish
            // lock freedom. If the CAS fails, we add some back off to statistically decrease
            // contention at the head, and then go back around and retry.

            Node newNode = new Node(item);
            newNode.m_next = m_head;
            if (Interlocked.CompareExchange(ref m_head, newNode, newNode.m_next) == newNode.m_next) {
                return;
            }

            // If we failed, go to the slow path and loop around until we succeed.
            PushCore(newNode, newNode);
        }

        private void PushCore(Node head, Node tail) {
            SpinWait spin = new SpinWait();

            // Keep trying to CAS the exising head with the new node until we succeed.
            do {
                spin.SpinOnce();
                // Reread the head and link our new node.
                tail.m_next = m_head;
            }
            while (Interlocked.CompareExchange(
                ref m_head, head, tail.m_next) != tail.m_next);
        }


        public bool TryPop(out T result) {
            Node head = m_head;
            //stack is empty
            if (head == null) {
                result = default(T);
                return false;
            }
            if (Interlocked.CompareExchange(ref m_head, head.m_next, head) == head) {
                result = head.m_value;
                return true;
            }

            // Fall through to the slow path.
            return TryPopCore(out result);
        }

        private bool TryPopCore(out T result) {
            Node poppedNode;

            if (TryPopCore(1, out poppedNode) == 1) {
                result = poppedNode.m_value;
                return true;
            }

            result = default(T);
            return false;

        }

        private int TryPopCore(int count, out Node poppedHead) {
            SpinWait spin = new SpinWait();

            // Try to CAS the head with its current next.  We stop when we succeed or
            // when we notice that the stack is empty, whichever comes first.
            Node head;
            Node next;
            int backoff = 1;
            Random r = new Random(Environment.TickCount & Int32.MaxValue); // avoid the case where TickCount could return Int32.MinValue
            while (true) {
                head = m_head;
                // Is the stack empty?
                if (head == null) {
                    poppedHead = null;
                    return 0;
                }
                next = head;
                int nodesCount = 1;
                for (; nodesCount < count && next.m_next != null; nodesCount++) {
                    next = next.m_next;
                }

                // Try to swap the new head.  If we succeed, break out of the loop.
                if (Interlocked.CompareExchange(ref m_head, next.m_next, head) == head) {
                    // Return the popped Node.
                    poppedHead = head;
                    return nodesCount;
                }

                // We failed to CAS the new head.  Spin briefly and retry.
                for (int i = 0; i < backoff; i++) {
                    spin.SpinOnce();
                }

                backoff = spin.NextSpinWillYield ? r.Next(1, BACKOFF_MAX_YIELDS) : backoff * 2;
            }
        }



    }
}
