testPredicate()
{
    iprintlnbold("testPredicate");
	//return int(str) == 10;
}

testPredicate2(str)
{
    iprintlnbold("testPredicate2");
	return int(str) == 10;
}

test()
{
    wait 10;
    arr = [];
    for (i = 0; i < 10; i++)
        arr[i] = i;

    //result = all(arr, ::testPredicate);

    //comPrintf(result + "\n");

    while (true)
    {
        testPtr(::testPredicate);
        //testPtr(::testPredicate2);
        wait 5;
    }
}

reverse(array)
{
    arr = [];
    index = 0;

    for (i = array.size; i != 0; i--)
    {
        arr[index] = array[i];
        index++;
    }
    return arr;
}

repeat(array, count)
{
    arr = [];
    index = 0;

    for (r = 0; r < count; r++)
    {
        for (i = 0; i < array.size; i++)
        {
            arr[index] = array[i];
            index++;
        }
    }
    return arr;
}

range(array, min, max)
{
    arr = [];
    index = 0;
    if (!isDefined(max))
        max = array.size;

    for (i = min; i < max; i++)
    {
        arr[index] = array[i];
        index++;
    }
    return arr;
}

where(array, predicate)
{
    arr = [];
    index = 0;

    for (i = 0; i < array.size; i++)
    {
        if ([[predicate]](array[i]))
        {
            arr[index] = array[i];
            index++;
        }
    }
    return arr;
}

select(array, func)
{
    arr = [];

    for (i = 0; i < array.size; i++)
        array[i] = [[func]](array[i]);
    return arr;
}

sum(array)
{
    ret = 0;
    for (i = 0; i < array.size; i++)
        ret += array[i];
    return ret;
}

count(array, predicate)
{
    ret = 0;
    for (i = 0; i < array.size; i++)
    {
        if ([[predicate]](array[i]))
            ret++;
    }
    return ret;
}

average(array)
{
    sum = array[0];
    count = 1;
    for (i = 0; i < array.size; i++)
    {
        sum += array[i];
        count++;
    }
    return int(sum / count); // todo float
}

getType(var)
{

}

orderBy(array)
{

}

sort(array)
{

}

sortType(array, type)
{

}

cast(array, type)
{

}

first(array, predicate)
{
    ret = undefined;
    for (i = 0; i < array.size; i++)
    {
        if ([[predicate]](array[i]))
            return array[i];
    }
    return ret;
}

last(array, predicate)
{
    ret = undefined;
    for (i = 0; i < array.size; i++)
    {
        if ([[predicate]](array[i]))
            ret = array[i];
    }
    return ret;
}

max(array)
{
    value = array[0];
    for (i = 0; i < array.size; i++)
    {
        if (array[i] > value)
            value = array[i];
    }
    return value;
}

min(array)
{
    value = array[0];
    for (i = 0; i < array.size; i++)
    {
        if (array[i] < value)
            value = array[i];
    }
    return value;
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
