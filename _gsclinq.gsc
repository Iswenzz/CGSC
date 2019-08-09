testPredicate(str)
{
	return int(str) == 10;
}

test()
{
    wait 10;
    arr = [];
    for (i = 0; i < 10; i++)
        arr[i] = i;

    result = all(arr, ::testPredicate);

    comPrintf(result + "\n");
}

any(array, predicate)
{
    for (i = 0; i < array.size; i++)
    {
        if ([[predicate]](array[i]))
            return true;
    }
    return false;
}

all(array, predicate)
{
    for (i = 0; i < array.size; i++)
    {
        if (![[predicate]](array[i]))
            return false;
    }
    return true;
}
