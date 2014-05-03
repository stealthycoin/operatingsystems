int main()
{
  int n = 50000, i = 3, count, c;
 
  for ( count = 2 ; count <= n ;  )
    {
      for ( c = 2 ; c <= i - 1 ; c++ )
	{
	  if ( i%c == 0 )
            break;
	}
      if ( c == i )
	{
	  count++;
	}
      i++;
    }
 
  return 0;
}
