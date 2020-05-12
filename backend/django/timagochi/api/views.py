from django.shortcuts import render
from rest_framework.views import APIView
from .serializers import CourseSerializer, PlayerSerializer, InfiniteScoreSerializer
from .models import Course, Player, InfiniteScore
from rest_framework import viewsets
from rest_framework.response import Response
from django.utils import timezone
from datetime import timedelta
from django.http import JsonResponse
from django.core import serializers


class InfiniteRunView(APIView):
    def post(self, request, format=None):
        name = request.data['name']
        score = int(request.data['score'])
        try:
            p = Player.objects.get(name = name)
            obj = InfiniteScore.objects.get(person = p)
            if score > obj.score:
                obj.score = score
                obj.save()
                return Response(score)
            return Response(obj.score)
        except InfiniteScore.DoesNotExist:
            obj = InfiniteScore(person=p, score = score)
            obj.save()
            return Response(score)


    def get(self, request):
        if 'name' in request.query_params:
            name = request.query_params['name']
            try:
                p = Player.objects.get(name = name)
                obj = InfiniteScore.objects.get(person=p)
                return Response(obj.score)
            except InfiniteScore.DoesNotExist:
                return Response(0)
        else:
            top3 = InfiniteScore.objects.order_by('-score')[:3]
            t3vl = top3.values_list("person_id", "score")
            l = list(t3vl)
            out = []
            for v in l:
                out.append(v[0])
                out.append(str(v[1]))

            return Response("&".join(out))
            return Response(t3vl)
            irs = InfiniteScoreSerializer(top3, many=True, context={'request': request})
            return Response(irs.data)




class TimagochiExistsView(APIView):
    def get(self, request):
        name = request.query_params['name']
        try: 
            obj = Player.objects.get(name=name)
            return Response(True)
        except Player.DoesNotExist:
            return Response(False)


class OthersOnlineView(APIView):
    def get(self, request):
        ten_mins = timezone.now() - timedelta(minutes = 10)
        lst = Player.objects.filter(last_seen__gte=ten_mins).values('name')

        return Response(lst)


class TimagochiView(APIView):
    def get(self, request):
        name = request.query_params['name']
        j = PlayerSerializer(Player.objects.get(name=name))
        return Response(j.data)

    def post(self, request, format=None):
        name = request.data['name']
        major = request.data['major']
        p = Player(name=name, major=major)
        p.save()
        return Response(True)


class CourseView(APIView):
    def post(self, request, format=None):
        name = request.data['name']
        person = Player.objects.get(name = name)
        course = request.data['class']
        add = request.data['add']
        if add.lower() == 'true':
            c = Course(person=person, course = course)
            c.save()
            p = Player.objects.get(name=name)
            p.last_seen = timezone.now()
            p.units = p.units + 1
            p.save()
        else:
            Course.objects.filter(person=person, course=course).delete()
            p = Player.objects.get(name=name)
            p.last_seen = timezone.now()
            p.units = p.units - 1
            p.save()
        return Response(True)


    def get(self, request, format=None):
        name = request.query_params['name']
        c = Course.objects.filter(person=name).values('course')
        return Response(c)
        l = CourseSerializer(c, many=True, context={'request': request})
        return Response(l.data)




class MajorView(APIView):
    def post(self, request, format=None):
        name = request.data['name']
        major = int(request.data['major'])
        person = Player.objects.get(name=name)
        person.major = major
        person.last_seen = timezone.now()
        person.save()
        return Response(True)
