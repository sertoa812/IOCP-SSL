#pragma once
#include <string>

namespace Queue{
	template<class T>
	class Node<T>
	{
		public T value = default(T);
		public Node<T> next = NULL;

		public Node(){}
	};

	template<class T>
	class Que<T>
	{
		private Node<T> head = NULL;
		private Node<T> rear = NULL;
		int count;

		public Que()
		{
			head = Node<T>();
			head.next = NULL;
			rear = head;
			count = 0;
		}

		public void Enqueue(T _value)
		{
			Node<T> p1 = Node<T>();
			p1.value = _value;
			p1.next = NULL;
			rear.next = p1;
			count++;
		}

		public T Dequeue()
		{
			if (count == 0 && head.next == NULL){
				return default(T);
			}
			Node<T> p = head.next;
			head.next = p.next;
			count--;
			if (count == 0)
			{
				rear = head;
			}
			return p.a;
		}
	};
}